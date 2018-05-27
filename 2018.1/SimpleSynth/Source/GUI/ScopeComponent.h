/*
  ==============================================================================

    ScopeComponent.h
    Created: 15 May 2018 1:03:51am
    Author:  COx2

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

template <typename SampleType>
class AudioBufferQueue
{
public:
	static constexpr size_t order = 9;
	static constexpr size_t bufferSize = 1U << order;
	static constexpr size_t numBuffers = 5;

	void push(const SampleType* dataToPush, size_t numSamples)
	{
		jassert(numSamples <= bufferSize);

		int start1, size1, start2, size2;
		abstractFifo.prepareToWrite(1, start1, size1, start2, size2);

		jassert(size1 <= 1);
		jassert(size2 == 0); //ステレオの場合はsize2 <= 1とすべきかもしれない?

		if (size1 > 0) 
		{
			FloatVectorOperations::copy(buffers[(size_t)start1].data(), dataToPush, (int)jmin(bufferSize, numSamples));
		}
	
		abstractFifo.finishedWrite(size1);

	}

	void pop(SampleType* outputBuffer)
	{
		int start1, size1, start2, size2;
		abstractFifo.prepareToRead(1, start1, size1, start2, size2);


		jassert(size1 <= 1);
		jassert(size2 == 0);

		if (size1 > 0) 
		{
			FloatVectorOperations::copy(outputBuffer, buffers[(size_t)start1].data(), (int)bufferSize);
		}

		abstractFifo.finishedRead(size1);

	}

private:
	AbstractFifo abstractFifo{ numBuffers };
	std::array <std::array<SampleType, bufferSize>, numBuffers> buffers;
};

// オシロスコープパネルに表示するサンプルデータを回収するコレクタークラス
template<typename SampleType>
class ScopeDataCollector
{
public:
	ScopeDataCollector(AudioBufferQueue<SampleType>& queueToUse)
		: audioBufferQueue(queueToUse)
	{}

	void process(const SampleType* data, size_t numSamples)
	{
		size_t index = 0;

		// トリガーレベルを待機する処理
		if (currentState == State::WaitingForTrigger)
		{
			while (index++ < numSamples)
			{
				auto currentSample = *data++;

				// 所定のレベル以上の値であればサンプリングするトリガーがオンになり、回収状態に移行する。
				// オシロスコープでいうトリガーモードに類似
				if (currentSample >= triggerLevel && prevSample < triggerLevel)
				{
					numCollected = 0;
					currentState = State::Collecting;
					break;
				}

				prevSample = currentSample;
			}
		}

		// 回収状態であるときの処理
		if (currentState == State::Collecting)
		{
			while (index++ < numSamples)
			{
				buffer[numCollected++] = *data++;

				if (numCollected == buffer.size())
				{
					audioBufferQueue.push(buffer.data(), buffer.size());
					currentState = State::WaitingForTrigger;
					prevSample = SampleType(100);
					break;
				}
			}
		}
	}

private:
	AudioBufferQueue<SampleType>& audioBufferQueue;
	std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
	size_t numCollected;
	SampleType prevSample = SampleType(100);

	static constexpr auto triggerLevel = SampleType(0.001);

	enum class State
	{
		WaitingForTrigger,
		Collecting
	};

	State currentState{ State::WaitingForTrigger };
};

template<typename SampleType>
class ScopeComponent : public juce::Component, private Timer
{
public:
	using Queue = AudioBufferQueue<SampleType>;

	ScopeComponent(Queue& queueuToUse)
		: audioBufferQueue(queueuToUse)
	{
		sampleData.fill(SampleType(0));
		setFramePerSecond(30);
	}

	void setFramePerSecond(int framePerSecond)
	{
		jassert(framePerSecond > 0 && framePerSecond < 1000);
		startTimerHz(framePerSecond);
	}

	void paint(Graphics& g) override
	{
		int panelNameHeight = 42;
		int localMargin = 2;
		Font panelNameFont = Font(24.0f, Font::plain).withTypefaceStyle("Italic");
		{
			float x = 0.0f, y = 0.0f, width = getWidth(), height = getHeight();
			Colour panelColour = juce::Colour(36, 36, 36);
			g.setColour(panelColour);
			g.fillRoundedRectangle(x, y, width, height, 10.0f);
		}

		{
			Rectangle<int> bounds = getLocalBounds(); // コンポーネント基準の値
			String text(TRANS("SCOPE"));
			Colour fillColour = Colours::white;
			g.setColour(fillColour);
			g.setFont(panelNameFont);
			g.drawText(text, bounds.removeFromTop(panelNameHeight).reduced(localMargin), Justification::centred, true);
		}

		
		Rectangle<int> drawArea = getLocalBounds();
		drawArea.removeFromTop(panelNameHeight);
		drawArea.reduce(drawArea.getWidth()* 0.05f, drawArea.getHeight()* 0.1f);
		g.setColour(juce::Colours::darkgrey);
		g.fillRect(drawArea);

		auto drawX = (SampleType)drawArea.getX();
		auto drawY = (SampleType)drawArea.getY();
		auto drawH = (SampleType)drawArea.getHeight();
		auto drawW = (SampleType)drawArea.getWidth();

		// Oscilloscopeを描画
		g.setColour(juce::Colours::cyan);
		auto scopeRect = Rectangle<SampleType>{ drawX, drawY, drawW, drawH / 1 };
		plot(sampleData.data(), sampleData.size(), g, scopeRect, SampleType(0.4), drawH / 2);


		// Spectrumを描画
		//g.setColour(juce::Colours::white);
		//auto spectrumRect = Rectangle<SampleType>{ drawX, drawH / 2, drawW, drawH / 2 };
		//plot(spectrumData.data(), spectrumData.size() / 4, g, spectrumRect);
	
	}

	void resized() override {}

private:
	Queue & audioBufferQueue;
	std::array<SampleType, Queue::bufferSize> sampleData;

	juce::dsp::FFT fft{ Queue::order };
	using WindowFunc = juce::dsp::WindowingFunction<SampleType>;
	WindowFunc windowFunc{ (size_t)fft.getSize(), WindowFunc::hann };
	std::array<SampleType, 2 * Queue::bufferSize> spectrumData;

	void timerCallback() override
	{
		audioBufferQueue.pop(sampleData.data());
		FloatVectorOperations::copy(spectrumData.data(), sampleData.data(), (int)sampleData.size());

		auto fftSize = (size_t)fft.getSize();

		jassert(spectrumData.size() == 2 * fftSize);
		windowFunc.multiplyWithWindowingTable(spectrumData.data(), fftSize);
		fft.performFrequencyOnlyForwardTransform(spectrumData.data());

		static constexpr auto mindB = SampleType(-160); //最小dB
		static constexpr auto maxdB = SampleType(0);	//最大dB

		for (auto& s : spectrumData) 
		{
			s = jmap(jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(s) - juce::Decibels::gainToDecibels(SampleType(fftSize)))
				, mindB
				, maxdB
				, SampleType(0)
				, SampleType(1)
			);
		}

		repaint();
	}

	static void plot(const SampleType* data
		, size_t numSamples
		, Graphics& g
		, juce::Rectangle<SampleType> rect
		, SampleType scaler = SampleType(1)
		, SampleType offset = SampleType(0)
	)
	{
		auto w = rect.getWidth();
		auto h = rect.getHeight();
		auto right = rect.getRight();
		auto alignedCentre = rect.getBottom() - offset;
		auto gain = h * scaler;

		for (size_t i = 1; i < numSamples; ++i)
		{
			// void Graphics::drawLine (float x1, float y1, float x2, float y2) const
			g.drawLine(
				{
					  jmap(SampleType(i - 1), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right))
					, alignedCentre - gain * data[i - 1]
					, jmap(SampleType(i), SampleType(0), SampleType(numSamples - 1), SampleType(right - w), SampleType(right))
					, alignedCentre - gain * data[i] 
				}
			);
		}

	}

};
