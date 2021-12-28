#ifndef __PREEMPT_H
#define __PREEMPT_H

static inline void preempt_disable(void)
{
	current->preempt_cnt++;
}

static inline void preempt_enable(void)
{
	current->preempt_cnt--;
}

#endif
