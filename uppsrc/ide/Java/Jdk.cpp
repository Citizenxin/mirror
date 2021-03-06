#include "Java.h"

namespace Upp {

#define METHOD_NAME "Jdk::" + String(__FUNCTION__) + "(): "

String Jdk::GetDownloadUrl()
{
	return "http://www.oracle.com/technetwork/java/javase/downloads/index.html";
}

Jdk::Jdk(const String& path, Host* host)
	: path(path)
{
	ASSERT_(host, "Host is null.");
	
	FindVersion(host);
}

bool Jdk::Validate() const
{
	if(!FileExists(GetJavacPath())) return false;
	
	return true;
}

void Jdk::FindVersion(Host* host)
{
	if (!Validate()) {
		LOG(METHOD_NAME + "Path to JDK is wrong or files are corrupted.");
		return;
	}
	
	StringStream ss;
	if (host->Execute(GetJavacPath() + " -version", ss) != 0) {
		LOG(METHOD_NAME + "Cannot obtain version due to command execution failure.");
		return;
	}
	
	String output = static_cast<String>(ss);
	output.Replace("\n", "");
	Vector<String> splitedOutput = Split(output, " ");
	if (splitedOutput.GetCount() != 2) {
		LOG(METHOD_NAME + "Splited output is too short (" + output + ").");
		return;
	}
	
	Vector<String> splitedVersion = Split(splitedOutput[1], ".");
	if (splitedVersion.GetCount() != 3) {
		LOG(METHOD_NAME + "Splited version is too short (" + output + ").");
		return;
	}
	
	int major = StrInt(splitedVersion[0]);
	if (major == INT_NULL) {
		LOG(METHOD_NAME + "Major version conversion to int failed (" + splitedVersion[0] + ").");
		return;
	}
	
	int minor = StrInt(splitedVersion[1]);
	if (minor == INT_NULL) {
		LOG(METHOD_NAME + "Minor version conversion to int failed (" + splitedVersion[1] + ").");
		return;
	}
	
	version = JavaVersion(major, minor);
}

}
