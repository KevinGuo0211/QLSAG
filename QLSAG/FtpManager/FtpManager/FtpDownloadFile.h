/************************************************************************
*   �ļ����ƣ�FtpDownloadFile.h
*   ��д��Ա��LHH
*   �������ڣ�2018/01/09
*   ����������FTP�ϴ�/�����ļ��ӿ�
*   ��    ע��
*   �޸�������
************************************************************************/
#ifndef FTPDOWNLOADFILE_H
#define FTPDOWNLOADFILE_H

#include "ftpmanager_global.h"

#include<QString>
#include<QList>
#include<QObject>

//�ϴ��ӿ��ڲ���
class FtpUploadOneFile
{
public:
	FtpUploadOneFile(QString ip, int port, QString user, QString pwd, QString serverpath, QString file);
	FtpUploadOneFile(QString id);//Listȡֵר��
	FtpUploadOneFile();
	QString getID() const;
	QString getIP();
	int getPort();
	QString getUser();
	QString getPwd();
	QString getServerPath();
	QString getFile();
	bool operator == (const FtpUploadOneFile &a);
	bool operator !=(const FtpUploadOneFile &a);
private:
	QString _id;
	QString _ip;
	int _port;
	QString _user;
	QString _pwd;
	QString _serverpath;
	QString _file;
};

//�ϴ��ӿ��ⲿ��
class FTPMANAGER_EXPORT FtpUploadFile
{
public:
	FtpUploadFile(QString ip, int port, QString user, QString pwd, QString serverpath);
	FtpUploadFile(QString ip, int port, QString user, QString pwd, QString serverpath, QList<QString> files);
	FtpUploadFile();
	QString getIP();
	int getPort();
	QString getUser();
	QString getPwd();
	QString getServerPath();
	QList<QString> getFiles();

	void setIP(QString ip);
	void setPort(int port);
	void setUser(QString user);
	void setPwd(QString pwd);
	void setServerPath(QString serverpath);
	void setFiles(QList<QString> files);
	QList<FtpUploadOneFile> getFtpUploadOneFile();
private:
	QString _ip;  //FTP IP
	int _port;  //FTP PORT
	QString _user; //FTP USER
	QString _pwd; //FTP PASSWORD
	QString _serverpath; //FTP PATH TO LOAD
	QList<QString> _files; //FULL PATH FILE -- LOCAL FILE Sample: C:\Qt\Qt5.6.2\5.6\msvc2013_64\include\QtFtp\XXX.h
};


//���ؽӿ��ڲ���
class FTPMANAGER_EXPORT FtpDownloadOneFile
{
public:
	FtpDownloadOneFile(QString ip, int port, QString user, QString pwd, QString serverpath, QString file);
	FtpDownloadOneFile(QString id);//Listȡֵר��
	FtpDownloadOneFile();
	bool operator == (const FtpDownloadOneFile &a);
	bool operator !=(const FtpDownloadOneFile &a);

	QString getID() const;
	QString getIP();
	int getPort();
	QString getUser();
	QString getPwd();
	QString getServerPath();
	QString getFile();
	
private:
	QString _id;
	QString _ip;
	int _port;
	QString _user;
	QString _pwd;
	QString _serverpath;
	QString _file;
};

//���ؽӿ��ⲿ��
class FTPMANAGER_EXPORT FtpDownloadFile
{
public:
	FtpDownloadFile(QString ip, int port, QString user, QString pwd, QString serverpath);
	FtpDownloadFile(QString ip, int port, QString user, QString pwd, QString serverpath, QList<QString> files);
	FtpDownloadFile();
	QString getIP();
	int getPort();
	QString getUser();
	QString getPwd();
	QString getServerPath();
	QList<QString> getFiles();

	void setIP(QString ip);
	void setPort(int port);
	void setUser(QString user);
	void setPwd(QString pwd);
	void setServerPath(QString serverpath);
	void setFiles(QList<QString> files);
	QList<FtpDownloadOneFile> getFtpDownloadOneFile();
private:
	QString _ip;  //FTP IP
	int _port;  //FTP PORT
	QString _user; //FTP USER
	QString _pwd; //FTP PASSWORD
	QString _serverpath; //FTP PATH TO LOAD
	QList<QString> _files; //FULL PATH FILE -- LOCAL FILE Sample: C:\Qt\Qt5.6.2\5.6\msvc2013_64\include\QtFtp\XXX.h
};



#endif // FTPDOWNLOADFILE_H
