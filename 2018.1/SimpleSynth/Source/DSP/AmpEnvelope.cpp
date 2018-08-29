/*
  ==============================================================================

    AmpEnvelope.cpp
    Created: 20 Apr 2018 2:08:26am
    Author:  COx2

  ==============================================================================
*/

// ヘッダファイルをインクルードする。
#include "AmpEnvelope.h"

// ①無名名前空間に定数宣言を記述する。
namespace 
{
	const float AMP_MAX = 1.0f;
	const float AMP_MIN = 0.0f;
	const float ATTACK_MIN = 0.01f;
	const float DECAY_MIN = 0.01f;
	const float RELEASE_MIN = 0.01f;
}

// ②引数付きコンストラクタ。初期化指定子にて引数で渡された各パラメータの初期値をクラス内変数に代入する。
AmpEnvelope::AmpEnvelope(float attackTime, float decayTime, float sustain, float releaseTime)
	: _attackTime(attackTime), _decayTime(decayTime), _sustainValue(sustain), _releaseTime(releaseTime)
	, _sampleRate(0.0f), _value(0.0f), _valueOnReleaseStart(0.0f), _ampState(AMPENV_STATE::WAIT)
{
	if (_attackTime <= ATTACK_MIN) 
	{
		_attackTime = ATTACK_MIN;
	}

	if (_decayTime <= DECAY_MIN) 
	{
		_decayTime = DECAY_MIN;
	}

	if (_releaseTime <= RELEASE_MIN) 
	{
		_releaseTime = RELEASE_MIN;
	}

	if (_sustainValue > AMP_MAX) 
	{
		_sustainValue = AMP_MAX;
	}

	if (_sustainValue < AMP_MIN) 
	{
		_sustainValue = AMP_MIN;
	}

}

// デストラクタ
AmpEnvelope::~AmpEnvelope()
{}

// ③エンベロープの現在の状態を返す関数。
AmpEnvelope::AMPENV_STATE  AmpEnvelope::getState()
{
	return _ampState;
}

// ④エンベロープの現在の値を返す関数。
float AmpEnvelope::getValue()
{
	return _value;
}

// ⑤エンベロープの各パラメータに値をセットする関数。
void AmpEnvelope::setParameters(float attackTime, float decayTime, float sustain, float releaseTime)
{
	_attackTime = attackTime;
	_decayTime = decayTime;
	_sustainValue = sustain;
	_releaseTime = releaseTime;

	if (_attackTime <= ATTACK_MIN)
	{
		_attackTime = ATTACK_MIN;
	}

	if (_decayTime <= DECAY_MIN)
	{
		_decayTime = DECAY_MIN;
	}

	if (_releaseTime <= RELEASE_MIN)
	{
		_releaseTime = RELEASE_MIN;
	}

	if (_sustainValue > AMP_MAX)
	{
		_sustainValue = AMP_MAX;
	}

	if (_sustainValue < AMP_MIN)
	{
		_sustainValue = AMP_MIN;
	}
}

// ⑥エンベロープをAttack状態に移行させる関数。
void AmpEnvelope::attackStart(float sampleRate)
{
	if (!isReleasing())
	{
		_value = AMP_MIN;
	}

	_sampleRate = sampleRate;
	_ampState = AMPENV_STATE::ATTACK;

}

// ⑦エンベロープをRelease状態に移行させる関数。
void AmpEnvelope::releaseStart()
{
	if (isHolding()) 
	{
		_ampState = AMPENV_STATE::RELEASE;
		_valueOnReleaseStart = _value;
	}

}

// ⑧エンベロープをWait状態に移行させる関数。
void AmpEnvelope::releaseEnd()
{
	_value = AMP_MIN;
	_ampState = AMPENV_STATE::WAIT;
}

// ⑨エンベロープの状態がAttack, Decay, Sustainのいずれかに該当するかを返す関数。
bool AmpEnvelope::isHolding()
{
	if (_ampState == AmpEnvelope::AMPENV_STATE::ATTACK
		|| _ampState == AmpEnvelope::AMPENV_STATE::DECAY
		|| _ampState == AmpEnvelope::AMPENV_STATE::SUSTAIN)
	{
		return true;
	}
	return false;

}

// ⑩エンベロープの状態がRelease状態かどうかを返す関数。
bool AmpEnvelope::isReleasing()
{
	return _ampState == AmpEnvelope::AMPENV_STATE::RELEASE;
}

// ⑪エンベロープの計算処理を1サンプル分進める。
//   この計算処理を実行することで各クラス内変数の値が更新される。
void AmpEnvelope::cycle()
{
	// 1サンプルごとのvalue値の変化
	switch (_ampState)
	{
	case AMPENV_STATE::ATTACK:							// Attack状態の時の更新処理
		_value += AMP_MAX / (_sampleRate * _attackTime);
		if (_value >= AMP_MAX)
		{
			_value = AMP_MAX;
			_ampState = AMPENV_STATE::DECAY;
		}
		break;

	case AMPENV_STATE::DECAY:							// Decay状態の時の更新処理
		_value -= AMP_MAX / (_sampleRate * _decayTime);
		if (_value <= _sustainValue)
		{
			_value = _sustainValue;
			_ampState = AMPENV_STATE::SUSTAIN;
		}
		break;

	case AMPENV_STATE::SUSTAIN:							// Sustain状態の時の更新処理
		_value = _sustainValue;
		break;

	case AMPENV_STATE::RELEASE:							// Release状態の時の更新処理
		_value -= _valueOnReleaseStart / (_sampleRate * _releaseTime);
		if (_value <= AMP_MIN)
		{
			_value = AMP_MIN;
			_ampState = AMPENV_STATE::WAIT;
		}
		break;

	case AMPENV_STATE::WAIT:							// Wait状態の時の更新処理
		_value = AMP_MIN;
		break;
	}
}