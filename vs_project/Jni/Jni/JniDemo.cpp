// Jni.cpp : 定义控制台应用程序的入口点。
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
	//先获取对应的java类
	jclass jclaz = env->GetObjectClass(job);

	//第二个参数对应的是java的变量名，第三个是类型的签名
	jfieldID fid = env->GetFieldID(jclaz, "key", "Ljava/lang/String;");

	char text[100] = "Jni change string field value ";

	if (fid == NULL) { // 如果字段为 NULL ，直接退出，查找失败
		return;
	}
	jstring jstr = (jstring)env->GetObjectField(job, fid); // 获取字段对应的值
	const char * str = env->GetStringUTFChars(jstr, NULL);

	strcat(text, str);

	jstring key = env->NewStringUTF(text);
	//修改key的值
	env->SetObjectField(job, fid, key);

	//释放资源
	//env->ReleaseStringUTFChars(key, text);

}

/*
* Class:     JniMain
* Method:    accessStaticField
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_JniMain_accessStaticField
(JNIEnv *env, jobject job) {
	//获取类名
	jclass jclaz = env->GetObjectClass(job);

	//获取静态字段
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
	
	// 获得具体的类
	jclass cls = env->GetObjectClass(animal); 
	// 获得具体的方法 id
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
	
	//获取具体的类
	jclass jclz = env->FindClass("Animal");//参数也类的路径名

	// 获取具体的静态方法的 id
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
	//通过类的路径来从JVM里面找到对应的类
	jclass jclz = env->FindClass("java/util/Date");
	//jmethodId  构造方法
	jmethodID jmid = env->GetMethodID(jclz, "<init>","()V");

	if (jmid == NULL) {
		return NULL;
	}

	// 调用newObject 实例化Date 对象，返回值是一个jobject
	jobject date_obj = env->NewObject(jclz, jmid);

	// 得到对应的对象方法，前提是，我们访问了相关对象的构造函数创建了这个对象
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
	//env->ReleaseStringChars(in, c_str);// JVM 使用。通知JVM c_str 所指的空间可以释放了
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

	 char * c_str = "马蓉与宝宝";
	//创建String的类
	jclass jclz = env->FindClass("java/lang/String");
	//获取构造函数的mid   这边使用的是java的String(byte[], string)的构造函数
	jmethodID mid = env->GetMethodID(jclz, "<init>", "([BLjava/lang/String;)V");

	//创建参数 jstring -> jbyteArray
	jbyteArray bytes = env->NewByteArray(strlen(c_str));

	// char * 赋值到byte数组中
	env->SetByteArrayRegion(bytes, 0, strlen(c_str), (jbyte*)c_str);
	// 设置编码
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
		// 让java 继续运行
		env->ExceptionDescribe();//输出关于这个异常的描述
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
	//数组长度
	int len = env->GetArrayLength(array);
	qsort(elemts, len, sizeof(jint), compare);
	//释放可能的内存
	//将JNI  修改的数据重新写回原来的内存
	env->ReleaseIntArrayElements(array, elemts, JNI_COMMIT);
}

/*
* Class:     JniMain
* Method:    initInt2DArray
* Signature: (I)[[I
*/
JNIEXPORT jobjectArray JNICALL Java_JniMain_initStringArray
(JNIEnv *env, jobject job, jint size) {
	//创建jobjectArray对象
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
	//赋值
	for ( i = 0; i < size; i++)
	{
		char * c_str = (char *)malloc(256);
		memset(c_str, 0, 256);
		//将 int 转换成为 char
		sprintf(c_str, "hello num: %d\n",i);
		// C -> jstring
		jstring str = env->NewStringUTF(c_str);
		if (str == NULL) {
			return NULL;
		}

		// 将jstring 赋值给数组
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
	//返回对象，是个二维数组
	jobjectArray ret;
	int i = 0;
	int j = 0;
	jclass intArrayClz = env->FindClass("[I");
	if (intArrayClz == NULL) {
		return NULL;
	}

	ret = env->NewObjectArray(size * 3, intArrayClz, NULL);

	jint tmp[3];//固定数组
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
* Method:    localRef   局部引用
* Signature: ()V
*/
JNIEXPORT void JNICALL Java_JniMain_localRef
(JNIEnv *env, jobject job) {
	int i;
	for ( i = 0; i < 5; i++)
	{
		jclass clz = env->FindClass("java/util/Date");
		jmethodID mid = env->GetMethodID(clz, "<init>", "()V");
		//创建对象
		jobject date_obj = env->NewObject(clz, mid);
		//使用这个引用
		jmethodID time_mid = env->GetMethodID(clz, "getTime", "()J");
		jlong time = env->CallLongMethod(date_obj, time_mid);

		printf("local reference time: %lld \n", time);

		//释放引用
		env->DeleteLocalRef(clz);
		env->DeleteLocalRef(date_obj);
	}
}
//局部引用
// 定义方式多样：FindClass，NewObject,GetObjectClass,NewCharArray.... NewLocalRef()
//释放方式： 1 方法调用完JVM 会自动释放 2.DeleteLocalRef
// 不能在多线程里面使用


/*
* Class:     JniMain
* Method:    createGlobalRef  创建全局引用
* Signature: ()V
*/
jstring global_str;
JNIEXPORT void JNICALL Java_JniMain_createGlobalRef
(JNIEnv *env, jobject job) {
	jstring str = env->NewStringUTF("JNI is intersting");
	global_str = (jstring)env->NewGlobalRef(str);
}
//全局引用
//跨线程，跨方法使用
// NewGlobalRef 是创建全局引用的唯一方法

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
* Method:    exception   异常处理
* Signature: ()V;
*/
JNIEXPORT void JNICALL Java_JniMain_exception
(JNIEnv *env, jobject job) {
	jclass cls = env->GetObjectClass(job);
	jfieldID fid = env->GetFieldID(cls, "key11", "Ljava/lang/String;");

	//检查是否发送异常
	jthrowable ex = env->ExceptionOccurred();
	// 判断异常是否发送
	if (ex != NULL) {
		jclass newExc;
		//清空JNI 产生的异常
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
