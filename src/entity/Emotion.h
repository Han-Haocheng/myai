//
// Created by HanHaocheng on 2025/1/13.
//

#ifndef EMOTION_H
#define EMOTION_H

#include "../define.h"

MYAI_BEGIN

class Emotion {
public:
	using ptr = std::shared_ptr<Emotion>;

private:
	weight_t m_emotion;
	weight_t m_focus;

};

MYAI_END

#endif //EMOTION_H
