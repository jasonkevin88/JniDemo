// Jni.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "JniMain.h"
#include <string.h>
#include <Windows.h>

/*
* Class:     JniMain
* Method:    getStringFromCPP
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_JniMain_getStringFromCPP
(JNIEnv *env, jclass jclaz) {
	return env->NewStringUTF("java static method call C++ back string");
}

/*
* Class:     JniMain
* Method:    getStringFromCPP2
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_JniMain_getStringFromCPP2
(JNIEnv *env, jobject job) {
	return env->NewStringUTF("java non-static method call C++ back string ");
}

/*
* Class:     JniMain
* Method:    accessField
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_JniMain_accessField
(JNIEnv *env, jobject job) {
	//�Ȼ�ȡ��Ӧ��java��
	jclass jclaz = env->GetObjectClass(job);

	//�ڶ���������Ӧ����java�ı������������������͵�ǩ��
	jfieldID fid = env->GetFieldID(jclaz, "key", "Ljava/lang/String;");

	char text[100] = "Jni change string field value ";

	if (fid == NULL) { // ����ֶ�Ϊ NULL ��ֱ���˳�������ʧ��
		return;
	}
	jstring jstr = (jstring)env->GetObjectField(job, fid); // ��ȡ�ֶζ�Ӧ��ֵ
	const char * str = env->GetStringUTFChars(jstr, NULL);

	strcat(text, str);

	jstring key = env->NewStringUTF(text);
	//�޸�key��ֵ
	env->SetObjectField(job, fid, key);

	//�ͷ���Դ
	//env->ReleaseStringUTFChars(key, text);

}

/*
* Class:     JniMain
* Method:    accessStaticField
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_JniMain_accessStaticField
(JNIEnv *env, jobject job) {
	//��ȡ����
	jclass jclaz = env->GetObjectClass(job);

	//��ȡ��̬�ֶ�
	jfieldID fid = env->GetStaticFieldID(jclaz, "count", "I");

	if (fid == NULL) {
		return;
	}

	jint count = env->GetStaticIntField(jclaz, fid);

	count = 20;

	env->SetStaticIntField(jclaz, fid, count);
}

/*
* Class:     JniMain
* Method:    callInstanceMethod
* Signature: (LAnimal;)V
*/
JNIEXPORT void JNICALL Java_JniMain_callInstanceMethod
(JNIEnv *env, jobject instance, jobject animal) {
	
	// ��þ������
	jclass cls = env->GetObjectClass(animal); 
	// ��þ���ķ��� id
	jmethodID mid = env->GetMethodID(cls, "setName", "(Ljava/lang/String;)V"); 
	
	if (mid == NULL) {
		return;
	}
	jstring name = env->NewStringUTF("baozi");
	env->CallVoidMethod(animal, mid, name);
}

/*
* Class:     JniMain
* Method:    callStaticMethod
* Signature: (LAnimal;)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_JniMain_callStaticMethod
(JNIEnv *env, jobject instance, jobject animal) {
	
	//��ȡ�������
	jclass jclz = env->FindClass("Animal");//����Ҳ���·����

	// ��ȡ����ľ�̬������ id
	jmethodID mid = env->GetStaticMethodID(jclz, "getUID", "(Ljava/lang/String;)Ljava/lang/String;");
	if (mid == NULL) {
		return env->NewStringUTF("method no found!");
	}
	jstring id = env->NewStringUTF("xxxxxx");
	jstring result = (jstring)env->CallStaticObjectMethod(jclz, mid, id);
	return result;
}

/*
* Class:     JniMain
* Method:    acceessConstructor
* Signature: (LAnimal;)Ljava/lang/Object;
*/
JNIEXPORT jobject JNICALL Java_JniMain_acceessConstructor
(JNIEnv *env, jobject job) {
	//ͨ�����·������JVM�����ҵ���Ӧ����
	jclass jclz = env->FindClass("java/util/Date");
	//jmethodId  ���췽��
	jmethodID jmid = env->GetMethodID(jclz, "<init>","()V");

	if (jmid == NULL) {
		return NULL;
	}

	// ����newObject ʵ����Date ���󣬷���ֵ��һ��jobject
	jobject date_obj = env->NewObject(jclz, jmid);

	// �õ���Ӧ�Ķ��󷽷���ǰ���ǣ����Ƿ�������ض���Ĺ��캯���������������
	jmethodID time_mid = env->GetMethodID(jclz, "getTime","()J");
	jlong time = env->CallLongMethod(date_obj, time_mid);

	printf("time: %lld \n", time);
	return date_obj;
}

/*
* Class:     JniMain
* Method:    acceessConstructor
* Signature: (LAnimal;)Ljava/lang/Object;
*/
JNIEXPORT jobject JNICALL Java_JniMain_chineseChars
(JNIEnv *env, jobject job, jstring in) {
	jboolean iscp;
	//const jchar  * c_str = env->GetStringChars(in, &iscp);
	const jchar * c_str = env->GetStringChars(in, &iscp);
	if (iscp == JNI_TRUE)
	{
		printf("is copy: JNI_TRUE\n");
	}
	else if (iscp == JNI_FALSE)
	{
		printf("is copy: JNI_FALSE\n");
	}

	int length = env->GetStringLength(in);
	const jchar * jcstr = env->GetStringChars(in, NULL);
	if (jcstr == NULL) {
		return NULL;
	}
	//jchar -> char
	char * rtn = (char *)malloc(sizeof(char) * 2 * length + 3);
	memset(rtn, 0, sizeof(char) * 2 * length + 3);
	int size = 0;
	size = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)jcstr, length, rtn, sizeof(char) * 2 * length + 3, NULL, NULL);
	/*if (size <= 0)
	{
	printf("size: 0 \n", rtn);
	return NULL;
	}*/

	//if (rtn != NULL) {
		//free(rtn);
		//rtn = NULL;
	//}
	//env->ReleaseStringChars(in, c_str);// JVM ʹ�á�֪ͨJVM c_str ��ָ�Ŀռ�����ͷ���
	printf("string: %s\n", rtn);

	return NULL;
}

/*
* Class:     JniMain
* Method:    chineseChars2
* Signature: (LAnimal;)Ljava/lang/Object;
*/
JNIEXPORT jobject JNICALL Java_JniMain_chineseChars2
(JNIEnv *env, jobject job, jstring in) {

	 char * c_str = "�����뱦��";
	//����String����
	jclass jclz = env->FindClass("java/lang/String");
	//��ȡ���캯����mid   ���ʹ�õ���java��String(byte[], string)�Ĺ��캯��
	jmethodID mid = env->GetMethodID(jclz, "<init>", "([BLjava/lang/String;)V");

	//�������� jstring -> jbyteArray
	jbyteArray bytes = env->NewByteArray(strlen(c_str));

	// char * ��ֵ��byte������
	env->SetByteArrayRegion(bytes, 0, strlen(c_str), (jbyte*)c_str);
	// ���ñ���
	jstring charsetName = env->NewStringUTF("GB2312");

	return env->NewObject(jclz, mid, bytes, charsetName);
}

/*
* Class:     JniMain
* Method:    AcessCacheNewString
* Signature: (LAnimal;)Ljava/lang/Object;
*/
JNIEXPORT jstring JNICALL Java_JniMain_AcessCacheNewString
(JNIEnv *env, jobject job) {
	static jclass cls_string = NULL;
	if (cls_string == NULL)
	{
		printf("Java_JniMain_AcessCache_newString out: \n");
		cls_string = env->FindClass("Refrence");
	}
	//Sleep(10);
	jmethodID jmid = env->GetMethodID(cls_string, "getRef", "(I)I");
	jthrowable ex = env->ExceptionOccurred();
	if (ex != NULL)
	{
		jclass newExc;
		// ��java ��������
		env->ExceptionDescribe();//�����������쳣������
		printf("C exceptions happend\n");
	}
	jint in = env->CallIntMethod(job, jmid, 1);
	printf("Java_JniMain_AcessCache_newString in:%d \n", in);
	return NULL;
}

int compare(const void *a, const void *b) {
	return *(int *)a - *(int *)b;
}

/*
* Class:     JniMain
* Method:    giveArray
* Signature: ([I)V
*/
JNIEXPORT void JNICALL Java_JniMain_giveArray
(JNIEnv *env, jobject job, jintArray array) {
	//jintArray -> jint *
	jint *elemts = env->GetIntArrayElements(array, NULL);
	if (elemts == NULL)
	{
		return;
	}
	//���鳤��
	int len = env->GetArrayLength(array);
	qsort(elemts, len, sizeof(jint), compare);
	//�ͷſ��ܵ��ڴ�
	//��JNI  �޸ĵ���������д��ԭ�����ڴ�
	env->ReleaseIntArrayElements(array, elemts, JNI_COMMIT);
}

/*
* Class:     JniMain
* Method:    initInt2DArray
* Signature: (I)[[I
*/
JNIEXPORT jobjectArray JNICALL Java_JniMain_initStringArray
(JNIEnv *env, jobject job, jint size) {
	//����jobjectArray����
	jobjectArray result;
	jclass jclz;
	int i;
	jclz = env->FindClass("java/lang/String");
	if (jclz == NULL) {
		return NULL;
	}
	result = env->NewObjectArray(size,jclz, job);
	if (result == NULL) {
		return NULL;
	}
	//��ֵ
	for ( i = 0; i < size; i++)
	{
		char * c_str = (char *)malloc(256);
		memset(c_str, 0, 256);
		//�� int ת����Ϊ char
		sprintf(c_str, "hello num: %d\n",i);
		// C -> jstring
		jstring str = env->NewStringUTF(c_str);
		if (str == NULL) {
			return NULL;
		}

		// ��jstring ��ֵ������
		env->SetObjectArrayElement(result, i, str);
		free(c_str);
		c_str = NULL;
	}

	return result;
}

/*
* Class:     JniMain
* Method:    initInt2DArray
* Signature: (I)[[I
*/
JNIEXPORT jobjectArray JNICALL Java_JniMain_initInt2DArray
(JNIEnv *env, jobject job, jint size) {
	//���ض����Ǹ���ά����
	jobjectArray ret;
	int i = 0;
	int j = 0;
	jclass intArrayClz = env->FindClass("[I");
	if (intArrayClz == NULL) {
		return NULL;
	}

	ret = env->NewObjectArray(size * 3, intArrayClz, NULL);

	jint tmp[3];//�̶�����
	for ( i = 0; i < size; i++)
	{
		jintArray intArr = env->NewIntArray(3);

		for ( j = 0; j < 3; j++)
		{
			tmp[j] = i + j;
		}

		env->SetIntArrayRegion(intArr, 0, 3, tmp);
		env->SetObjectArrayElement(ret, i, intArr);
		env->DeleteLocalRef(intArr);
	}
	return ret;
}

/*
* Class:     JniMain
* Method:    localRef   �ֲ�����
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_JniMain_localRef
(JNIEnv *env, jobject job) {
	int i;
	for ( i = 0; i < 5; i++)
	{
		jclass clz = env->FindClass("java/util/Date");
		jmethodID mid = env->GetMethodID(clz, "<init>", "()V");
		//��������
		jobject date_obj = env->NewObject(clz, mid);
		//ʹ���������
		jmethodID time_mid = env->GetMethodID(clz, "getTime", "()J");
		jlong time = env->CallLongMethod(date_obj, time_mid);

		printf("local reference time: %lld \n", time);

		//�ͷ�����
		env->DeleteLocalRef(clz);
		env->DeleteLocalRef(date_obj);
	}
}
//�ֲ�����
// ���巽ʽ������FindClass��NewObject,GetObjectClass,NewCharArray.... NewLocalRef()
//�ͷŷ�ʽ�� 1 ����������JVM ���Զ��ͷ� 2.DeleteLocalRef
// �����ڶ��߳�����ʹ��


/*
* Class:     JniMain
* Method:    createGlobalRef  ����ȫ������
* Signature: ()V
*/
jstring global_str;
JNIEXPORT void JNICALL Java_JniMain_createGlobalRef
(JNIEnv *env, jobject job) {
	jstring str = env->NewStringUTF("JNI is intersting");
	global_str = (jstring)env->NewGlobalRef(str);
}
//ȫ������
//���̣߳��緽��ʹ��
// NewGlobalRef �Ǵ���ȫ�����õ�Ψһ����

/*
* Class:     JniMain
* Method:    getglobalRef
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_JniMain_getglobalRef
(JNIEnv *env, jobject job) {
	return global_str;
}

/*
* Class:     JniMain
* Method:    delGlobalRef
* Signature: ()V;
*/
JNIEXPORT void JNICALL Java_JniMain_delGlobalRef
(JNIEnv *env, jobject job) {
	env->DeleteGlobalRef(global_str);
}

/*
* Class:     JniMain
* Method:    createWeakRef
* Signature: ()Ljava/lang/String;
*/
jstring g_weak_cls;
JNIEXPORT jstring JNICALL Java_JniMain_createWeakRef
(JNIEnv *env, jobject job) {
	jclass cls_string = env->FindClass("java/lang/String");
	g_weak_cls = (jstring)env->NewWeakGlobalRef(cls_string);
	g_weak_cls = env->NewStringUTF("Jni weak reference");
	printf("weak ref = %s \n ",g_weak_cls);
	return g_weak_cls;
}

/*
* Class:     JniMain
* Method:    exception   �쳣����
* Signature: ()V;
*/
JNIEXPORT void JNICALL Java_JniMain_exception
(JNIEnv *env, jobject job) {
	jclass cls = env->GetObjectClass(job);
	jfieldID fid = env->GetFieldID(cls, "key11", "Ljava/lang/String;");

	//����Ƿ����쳣
	jthrowable ex = env->ExceptionOccurred();
	// �ж��쳣�Ƿ���
	if (ex != NULL) {
		jclass newExc;
		//���JNI �������쳣
		env->ExceptionClear();
		//NullPointerException
		newExc = env->FindClass("java/lang/IllegalArgumentException");
		if (newExc == NULL)
		{
			printf("exception\n");
			return;
		}
		env->ThrowNew(newExc, "Throw exception from JNI: GetFieldID faild ");
	}
}
