#include <stdint.h>
#include <jni.h>
#include "aubio.h"


struct data {
	aubio_pitch_t *o;
	fvec_t *input;
	fvec_t *pitch;
};


struct data data;


void Java_com_filipetrovic_auxilium_TunerUtils_Tuner_initPitch(	JNIEnv *env,
																	jobject obj, 
																	jint sampleRate, 
																	jint bufferSize) {

	uint32_t win = (uint32_t) bufferSize;
	uint32_t hop = win / 4;
	uint32_t samplerate = (uint32_t) sampleRate;

	data.o = new_aubio_pitch("yinfast", win, hop, samplerate);
	data.input = new_fvec(hop);
	data.pitch = new_fvec(1);

	aubio_pitch_set_tolerance(data.o, 0.10);
	aubio_pitch_set_unit(data.o, "Hz");
}


jfloat Java_com_filipetrovic_auxilium_TunerUtils_Tuner_getPitch(	JNIEnv *env,
																	jobject obj,
																	jshortArray inputArray) {


	jsize len = (*env)->GetArrayLength(env, inputArray);
    if (len != data.input->length)
		return len;

	jshort *body = (*env)->GetShortArrayElements(env, inputArray, 0);

    #pragma omp parallel for
	for (uint_t i = 0; i < len; i++)
		fvec_set_sample(data.input, body[i], i);
	
	(*env)->ReleaseShortArrayElements(env, inputArray, body, 0);


	if (aubio_silence_detection(data.input, 45) == 0) {
		aubio_pitch_do(data.o, data.input, data.pitch);
		return fvec_get_sample(data.pitch, 0);
	} 

	else
		return 0;
}


void Java_com_filipetrovic_auxilium_TunerUtils_Tuner_disposePitch(	JNIEnv *env,
																		jobject obj) {

	del_aubio_pitch(data.o);
	del_fvec(data.pitch);
	del_fvec(data.input);
	aubio_cleanup();
}
