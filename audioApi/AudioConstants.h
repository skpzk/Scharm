#ifndef audioconstants_h_
#define audioconstants_h_

#define SAMPLE_RATE         (44100)
#define TABLE_SIZE          (3200)
#define MAX_TABLE_NUM       (4)

// #define FRAMES_PER_BUFFER   (44100)
#define FRAMES_PER_BUFFER   (512)
// #define FRAMES_PER_BUFFER   (256)

#define CHANNELS            (2)
#define VOICES              (16)

#define FORMAT      paInt32
typedef int   sample_t;
#define SILENCE     ((sample_t)0x00)
// #define MAX         ((sample_t)1<<30)
#define MAX         ((sample_t)1<<27)

#define SINE     (0)
#define TRI      (1)
#define SAW      (2)
#define SQR      (4)
#define PURE_SQR (10)


#endif