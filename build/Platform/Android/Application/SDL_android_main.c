#ifdef __ANDROID__

#include <string.h>
#include <stdlib.h>
#include <jni.h>

int main(int argc, char *args[]);

JNIEXPORT int JNICALL Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject array)
{
	int i;
	int argc;
	int status;

	int len = (*env)->GetArrayLength(env, array);
	char* argv[1 + len + 1];
	argc = 0;
	/* Use the name "app_process" so PHYSFS_platformCalcBaseDir() works.
	https://bitbucket.org/MartinFelis/love-android-sdl2/issue/23/release-build-crash-on-start
	*/

	argv[argc++] = strdup("app_process");

	for (i = 0; i < len; ++i) {
		const char* utf;
		char* arg = NULL;
		jstring string = (*env)->GetObjectArrayElement(env, array, i);
		if (string) {
			utf = (*env)->GetStringUTFChars(env, string, 0);
			if (utf) {
				arg = strdup(utf);
				(*env)->ReleaseStringUTFChars(env, string, utf);
			}
			(*env)->DeleteLocalRef(env, string);
		}
		if (!arg) {
			arg = strdup("");
		}
		argv[argc++] = arg;
	}
	argv[argc] = NULL;


	status = main(argc, argv);

	for (i = 0; i < argc; ++i) {
		free(argv[i]);
	}

	exit(status);
	return status;
}

#endif 