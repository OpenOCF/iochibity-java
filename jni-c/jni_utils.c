#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void print_class_name(jobject obj)
{
    jclass cls = env->GetObjectClass(obj);
    /* print the class name */
    /* http://stackoverflow.com/questions/12719766/can-i-know-the-name-of-the-class-that-calls-a-jni-c-method */
    // First get the class object
    jmethodID mid = (*env)->GetMethodID(env, g_clazz, "getClass", "()Ljava/lang/Class;");
    jobject clsObj = (*env)->CallObjectMethod(env, j_resource_service_provider, mid);

    // Now get the class object's class descriptor
    jclass cls = (*env)->GetObjectClass(env, clsObj);

    // Find the getName() method on the class object
    mid = (*env)->GetMethodID(env, cls, "getName", "()Ljava/lang/String;");

    // Call the getName() to get a jstring object back
    jstring strObj = (jstring)(*env)->CallObjectMethod(env, clsObj, mid);

    // Now get the c string from the java jstring object
    const char* str = (*env)->GetStringUTFChars(env, strObj, NULL);

    // Print the class name
    printf("\nj_resource_service_provider class is: %s\n", str);
    // Release the memory pinned char array
    (*env)->ReleaseStringUTFChars(env,strObj, str);
}
