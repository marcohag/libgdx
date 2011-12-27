#include <com.badlogic.gdx.audio.analysis.KissFFT.h>

(JNIEnv* env, jclass clazz, jint numSamples) {

	KissFFT* fft = new KissFFT();
	fft->config = kiss_fftr_alloc(numSamples,0,NULL,NULL);
	fft->spectrum = (kiss_fft_cpx*)malloc(sizeof(kiss_fft_cpx) * numSamples);
	fft->numSamples = numSamples;
	return (jlong)fft;
	
}
JNIEXPORT void JNICALL Java_com_badlogic_gdx_audio_analysis_KissFFT_destroy
(JNIEnv* env, jclass clazz, jlong handle) {

	KissFFT* fft = (KissFFT*)handle;
	free(fft->config);
	free(fft->spectrum);
	free(fft);	
	
}
JNIEXPORT void JNICALL Java_com_badlogic_gdx_audio_analysis_KissFFT_spectrum
(JNIEnv* env, jclass clazz, jlong handle, jobject obj_samples, jobject obj_spectrum) {
	char* samples = (char*)env->GetDirectBufferAddress(obj_samples);
	char* spectrum = (char*)env->GetDirectBufferAddress(obj_spectrum);

	KissFFT* fft = (KissFFT*)handle;
	kiss_fftr( fft->config, (kiss_fft_scalar*)samples, fft->spectrum );

	int len = fft->numSamples / 2 + 1;
	float* out = (float*)spectrum;

	for( int i = 0; i < len; i++ )
	{
		float re = scale(fft->spectrum[i].r) * fft->numSamples;
		float im = scale(fft->spectrum[i].i) * fft->numSamples;

		if( i > 0 )
			out[i] = sqrtf(re*re + im*im) / (fft->numSamples / 2);
		else
			out[i] = sqrtf(re*re + im*im) / (fft->numSamples);
	}
	
}
JNIEXPORT void JNICALL Java_com_badlogic_gdx_audio_analysis_KissFFT_getRealPart
(JNIEnv* env, jclass clazz, jlong handle, jobject obj_real) {
	char* real = (char*)env->GetDirectBufferAddress(obj_real);

	KissFFT* fft = (KissFFT*)handle;
	short* out = (short*)real;
	for( int i = 0; i < fft->numSamples / 2; i++ )
		out[i] = fft->spectrum[i].r;
	
}
JNIEXPORT void JNICALL Java_com_badlogic_gdx_audio_analysis_KissFFT_getImagPart
(JNIEnv* env, jclass clazz, jlong handle, jobject obj_imag) {
	char* imag = (char*)env->GetDirectBufferAddress(obj_imag);

	KissFFT* fft = (KissFFT*)handle;
	short* out = (short*)imag;
	for( int i = 0; i < fft->numSamples / 2; i++ )
		out[i] = fft->spectrum[i].i;
	
}