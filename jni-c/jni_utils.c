#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "jni_utils.h"

/* print the class name */
/* http://stackoverflow.com/questions/12719766/can-i-know-the-name-of-the-class-that-calls-a-jni-c-method */

void print_class_name(JNIEnv* env, jobject obj)
{
    jclass clazz = (*env)->GetObjectClass(env,obj);
    // First get the class object
    jmethodID mid_getClass = (*env)->GetMethodID(env, clazz, "getClass", "()Ljava/lang/Class;");
    jobject clsObj = (*env)->CallObjectMethod(env, obj, mid_getClass);

    // Now get the class object's class descriptor
    jclass cls = (*env)->GetObjectClass(env, clsObj);

    // Find the getName() method on the class object
    jmethodID mid_getName = (*env)->GetMethodID(env, cls, "getName", "()Ljava/lang/String;");

    // Call the getName() to get a jstring object back
    jstring strObj = (jstring)(*env)->CallObjectMethod(env, clsObj, mid_getName);

    // Now get the c string from the java jstring object
    const char* str = (*env)->GetStringUTFChars(env, strObj, NULL);

    // Print the class name
    printf("\t%s\n", str);
    // Release the memory pinned char array
    (*env)->ReleaseStringUTFChars(env,strObj, str);
}
