/*
 * ArcEmu MMORPG Server
 * Copyright (C) 2008-2011 <http://www.ArcEmu.org/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../include/molnet/Log.h"
#include <cstdarg>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

static Mutex m_logLock;                                 /**< 日志锁，避免同时操作资源 */

std::string FormatOutputString(const char* Prefix, const char* Description, bool useTimeStamp)
{

	char p[MAX_PATH];
	p[0] = 0;
	time_t t = time(NULL);
	tm* a = gmtime(&t);
	strcat(p, Prefix);
	strcat(p, "/");
	strcat(p, Description);
	if(useTimeStamp)
	{
		char ftime[100];
		snprintf(ftime, 100, "-%-4d-%02d-%02d %02d-%02d-%02d", a->tm_year + 1900, a->tm_mon + 1, a->tm_mday, a->tm_hour, a->tm_min, a->tm_sec);
		strcat(p, ftime);
	}

	strcat(p, ".log");
	return std::string(p);
}

int CreateDir(const char *sPathName)
{
  char DirName[256];
  strcpy(DirName, sPathName);
  int i,len = strlen(DirName);
  for(i=1; i<len; i++)
  {
	  if(DirName[i]=='/')
	  {
		  DirName[i] = 0;
		  if(access(DirName, NULL)!=0)
		  {
			  if(mkdir(DirName, 0755)==-1)
			  {
				  printf("mkdir   error\n");
				  return -1;
			  }
		  }
		  DirName[i] = '/';

	  }
  }

  return 0;
}

createFileSingleton(oLog);

time_t UNIXTIME;
tm g_localTime;

void oLog::outFile(FILE* file, char* msg, const char* source)
{
    m_logLock.Acquire();
	char time_buffer[TIME_FORMAT_LENGTH];
	//char szltr_buffer[SZLTR_LENGTH];
	Time(time_buffer);
	//pdcds(SZLTR, szltr_buffer);

	if(source != NULL)
	{
		fprintf(file, "%s%s: %s\n", time_buffer, source, msg);
		printf("%s%s: %s\n", time_buffer, source, msg);
	}
	else
	{
		fprintf(file, "%s%s\n", time_buffer, msg);
		printf("%s%s\n", time_buffer, msg);
	}

	fflush(file);

	m_logLock.Release();
}

void oLog::Time(char* buffer)
{
	time_t now;
	struct tm* timeinfo = NULL;

	time(&now);
	timeinfo = localtime(&now);

	if(timeinfo != NULL)
	{
		strftime(buffer, TIME_FORMAT_LENGTH, TIME_FORMAT, timeinfo);
	}
	else
	{
		buffer[0] = '\0';
	}
}

void oLog::outString(const char* str, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outError(const char* err, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, err);
	vsnprintf(buf, 32768, err, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::outBasic(const char* str, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDetail(const char* str, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::outDebug(const char* str, ...)
{
	if(m_fileLogLevel < 2 || m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, str);
	vsnprintf(buf, 32768, str, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::logBasic(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	snprintf(message, 32768, " [BSC] %s:%d %s %s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::logDetail(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if((m_fileLogLevel < 1) || (m_normalFile == NULL))
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	snprintf(message, 32768, " [DTL] %s:%d %s %s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_normalFile, buf);
}

void oLog::logError(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	snprintf(message, 32768, " [ERR] %s:%d %s %s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

void oLog::logDebug(const char* file, int line, const char* fncname, const char* msg, ...)
{
	if((m_fileLogLevel < 2) || (m_errorFile == NULL))
		return;

	char buf[ 32768 ];
	char message[ 32768 ];

	snprintf(message, 32768, " [DBG] %s:%d %s %s", file, line, fncname, msg);
	va_list ap;

	va_start(ap, msg);
	vsnprintf(buf, 32768, message, ap);
	va_end(ap);

	outFile(m_errorFile, buf);
}

//old NGLog.h methods
void oLog::Notice(const char* source, const char* format, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Warning(const char* source, const char* format, ...)
{
	if(m_fileLogLevel < 1 || m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Success(const char* source, const char* format, ...)
{
	if(m_normalFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_normalFile, buf, source);
}

void oLog::Error(const char* source, const char* format, ...)
{
	if(m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_errorFile, buf, source);
}

void oLog::Debug(const char* source, const char* format, ...)
{
	if(m_fileLogLevel < 2 || m_errorFile == NULL)
		return;

	char buf[32768];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, 32768, format, ap);
	va_end(ap);

	outFile(m_errorFile, buf, source);
}

void oLog::LargeErrorMessage(const char* source, ...)
{
	std::vector<char*> lines;
	char* pointer;
	va_list ap;
	va_start(ap, source);

	pointer = const_cast<char*>(source);
	lines.push_back(pointer);

	size_t i, j, k;
	pointer = va_arg(ap, char*);
	while(pointer != NULL)
	{
		lines.push_back(pointer);
		pointer = va_arg(ap, char*);
	}

	outError("*********************************************************************");
	outError("*                        MAJOR ERROR/WARNING                        *");
	outError("*                        ===================                        *");

	for(std::vector<char*>::iterator itr = lines.begin(); itr != lines.end(); ++itr)
	{
		std::stringstream sstext;
		i = strlen(*itr);
		j = (i <= 65) ? 65 - i : 0;
		sstext << "* " << *itr;
		for(k = 0; k < j; ++k)
		{
			sstext << " ";
		}

		sstext << " *";
		outError(sstext.str().c_str());
	}

	outError("*********************************************************************");
}

void oLog::Init(int32 fileLogLevel, LogType logType,const char *logfile)
{
    m_logLock.Acquire();
	SetFileLoggingLevel(fileLogLevel);

	std::string logNormalFilename, logErrorFilename;
	char str[1024];

	char *dir = (char *)get_current_dir_name();
	sprintf(str,"%s/logs/",dir);
	CreateDir(str);

	switch(logType)
	{
		case LOGON_LOG:
			{
				logNormalFilename += "logon-normal.log";
				logErrorFilename += "logon-error.log";
				break;
			}
		case WORLD_LOG:
			{
				logNormalFilename += "world-normal.log";
				logErrorFilename += "world-error.log";
				break;
			}
	}

	if(logfile != 0)
	{
		logNormalFilename = str;
		logErrorFilename = str;

		sprintf(str,"%s_normal.log",logfile);
		logNormalFilename += str;
		sprintf(str,"%s_error.log",logfile);
		logErrorFilename += str;
	}

	m_normalFile = fopen(logNormalFilename.c_str(), "a");
	if(m_normalFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logNormalFilename.c_str(), strerror(errno));
	else
	{
		setbuf(m_normalFile,NULL);

		time_t nSeconds;
		time(&nSeconds);
		tm* aTm = localtime(&nSeconds);
		outBasic("[%-4d-%02d-%02d %02d:%02d:%02d] opening '%s'", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec,logNormalFilename.c_str());
	}

	m_errorFile = fopen(logErrorFilename.c_str(), "a");
	if(m_errorFile == NULL)
		fprintf(stderr, "%s: Error opening '%s': %s\n", __FUNCTION__, logErrorFilename.c_str(), strerror(errno));
	else
	{
		setbuf(m_errorFile,NULL);

		time_t nSeconds;
		time(&nSeconds);
		tm* aTm = localtime(&nSeconds);
		outError("[%-4d-%02d-%02d %02d:%02d:%02d] opening '%s'", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec,logErrorFilename.c_str());
	}
	m_logLock.Release();
}

void oLog::Close()
{
	if(m_normalFile != NULL)
	{
		fflush(m_normalFile);
		fclose(m_normalFile);
		m_normalFile = NULL;
	}

	if(m_errorFile != NULL)
	{
		fflush(m_errorFile);
		fclose(m_errorFile);
		m_errorFile = NULL;
	}
}

void oLog::SetFileLoggingLevel(int32 level)
{
	//log level -1 is no more allowed
	if(level >= 0)
		m_fileLogLevel = level;
}

void SessionLogWriter::write(const char* format, ...)
{
	if(!m_file)
		return;

	char out[32768];
	va_list ap;

	va_start(ap, format);
	time_t t = time(NULL);
	tm* aTm = localtime(&t);
	sprintf(out, "[%-4d-%02d-%02d %02d:%02d:%02d] ", aTm->tm_year + 1900, aTm->tm_mon + 1, aTm->tm_mday, aTm->tm_hour, aTm->tm_min, aTm->tm_sec);
	size_t l = strlen(out);
	vsnprintf(&out[l], 32768 - l, format, ap);
	fprintf(m_file, "%s\n", out);
	va_end(ap);
}

void SessionLogWriter::Open()
{
	m_file = fopen(m_filename, "a");
}

void SessionLogWriter::Close()
{
	if(!m_file) return;
	fflush(m_file);
	fclose(m_file);
	m_file = NULL;
}

SessionLogWriter::SessionLogWriter(const char* filename, bool open)
{
	m_filename = strdup(filename);
	m_file = NULL;
	if(open)
		Open();
}

SessionLogWriter::~SessionLogWriter()
{
	if(m_file)
		Close();

	free(m_filename);
}
