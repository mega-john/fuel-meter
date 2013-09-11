// the values are calculated with AvrWiz for ATmega8 @ 16 mhz
#ifndef timer_h__
#define timer_h__

#define TIMER0_16ms
#define TIMER1_1s

#ifdef TIMER0_05us//1mHz
#define TIMER_0_INITIAL_VALUE	248U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_1us//1mHz
#define TIMER_0_INITIAL_VALUE	240U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_2us//500kHz
#define TIMER_0_INITIAL_VALUE	224U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_3us//500kHz
#define TIMER_0_INITIAL_VALUE	208U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_4us//500kHz
#define TIMER_0_INITIAL_VALUE	192U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_5us//200kHz
#define TIMER_0_INITIAL_VALUE	176U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_8us
#define TIMER_0_INITIAL_VALUE	128U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_10us//100kHz
#define TIMER_0_INITIAL_VALUE	96U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_16us//100kHz
#define TIMER_0_INITIAL_VALUE	0U
#define PRESCALLER0 (1<<CS00) // prescaler = 1
#endif

#ifdef TIMER0_100us//10kHz
#define TIMER_0_INITIAL_VALUE	56U
#define PRESCALLER0 (1<<CS01) // prescaler = 8
#endif

#ifdef TIMER0_500us//2kHz
#define TIMER_0_INITIAL_VALUE	131U
#define PRESCALLER0 ((1<<CS01) | (1<<CS00)) // prescaler = 64
#endif

#ifdef TIMER0_1ms
#define TIMER_0_INITIAL_VALUE	6U
#define PRESCALLER0 ((1<<CS01) | (1<<CS00)) // prescaler = 1024
#endif

/*8 ms*/
#ifdef TIMER0_8ms
#define TIMER_0_INITIAL_VALUE	131U
#define PRESCALLER0 ((1<<CS02) | (1<<CS00)) // prescaler = 1024
#endif

#ifdef TIMER0_16ms
#define TIMER_0_INITIAL_VALUE	6U
#define PRESCALLER0 ((1<<CS02) | (1<<CS00)) // prescaler = 1024
#endif

/*
с делителем на 8, один тик таймера равен 0.5us на частоте 16mHz
переполнение каждые 32.768ms
*/
#ifdef TIMER1_32768ms//1000Hz
#define TIMER_1_INITIAL_VALUE	0UL
#define PRESCALLER1 ((1<<CS11)) // prescaler = 8
#endif

#ifdef TIMER1_1s
#define TIMER_1_INITIAL_VALUE 0x0BDCUL
#define PRESCALLER1 ((1<<CS12)); // prescaler = 256
#endif

void init_timers(void);

#endif // timer_h__
