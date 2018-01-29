/*********************************************************************************
*Auther:		YASAIKUN@Nanjing
*Date:  		20170415
*Description:	FTP�ϴ�ʵ����
**********************************************************************************/
#include "ftptool.h"
#include <QUuid>  
#include <QTextCodec>

FtpTool* FtpTool::_instance = NULL;
FtpTool* FtpTool::getInstance(){

    if(NULL==_instance){
        _instance = new FtpTool();
    }
    return _instance;
}
FtpTool::FtpTool():QObject()
{
    this->_timer1 = new QTimer();
    QObject::connect(this->_timer1,SIGNAL(timeout()),this,SLOT(timeout()));
    this->_timer1->start(10000);
    for(int i=0;i<10;i++){
       FtpThread* uthread =  new FtpThread();
	   QObject::connect(uthread, SIGNAL(end(FtpThread*)), this, SLOT(threadend(FtpThread*)));
	   QObject::connect(uthread, SIGNAL(success(QString)), this, SLOT(runsuccess(QString)));
	   QObject::connect(uthread, SIGNAL(error(QString, QString)), this, SLOT(runerror(QString, QString)));
	   QObject::connect(uthread, SIGNAL(progress(QString, qint64, qint64)), this, SLOT(runprogress(QString, qint64, qint64)));

	   this->_freethreads.append(uthread);
    }
}
FtpTool::~FtpTool(){
    for(int i=0;i<10;i++){
        delete this->_freethreads[i];
    }
    delete this->_timer1;
}

/* *Summary: �ϴ��ɹ���slot*/
void FtpTool::runsuccess(QString id){
    emit this->success(id);
}
/* *Summary: �쳣����erro��slot*/
void FtpTool::runerror(QString id,QString message){
    emit this->error(id  ,message );
}
/* *Summary: �����ϴ����ȵ�slot*/
void FtpTool::runprogress(QString id,qint64 bytesSent, qint64 bytesTotal){
    emit this->progress(id,bytesSent,bytesTotal);
}

/* *Summary: ��ʱ����*/
void FtpTool::timeout(){
    QMutexLocker locker(&mutex);
        for(int i=this->_freethreads.length()-1;i>=0;i--){
            FtpThread *uthread = this->_freethreads[i];
            if(this->_uploadfiles.length()>0){
            uthread ->upload(this->_uploadfiles[0]);
            this->_uploadfiles.removeAt(0);
            this->_usedthreads.append(uthread);
            this->_freethreads.removeAt(i);
            }
            else{
                break;
            }
        }
}

/* *Summary: �߳�end��ʱ��Ĵ���*/
void FtpTool::threadend(FtpThread *thread){
    QMutexLocker locker(&mutex);
    if(this->_uploadfiles.length()>0){
        thread ->upload(this->_uploadfiles[0]);
        this->_uploadfiles.removeAt(0);
    }else
    {
        this->_freethreads.append(thread);
        this->_usedthreads.removeOne(thread);
    }
}


/*
*Summary: �ϴ�����ļ�����̨FTP SERVER
*Parameters:
*     files: ��Ҫ�ϴ����ļ��б�����FTP HOST IP PORT �ļ�����·�� ����Ϣ��
*Return : void
*/
void FtpTool::putFileToServer(QList<FtpUploadFile> files){
        QMutexLocker locker(&mutex);
        FtpUploadFile filelist;
        foreach (filelist, files) {
                this->_uploadfiles.append(filelist.getFtpUploadOneFile());
       }
       qDebug()<<"put OK";
}





FtpDTool* FtpDTool::_instance = NULL;
FtpDTool* FtpDTool::getInstance(){

	if (NULL == _instance){
		_instance = new FtpDTool();
	}
	return _instance;
}
FtpDTool::FtpDTool() :QObject()
{
	this->_timer2 = new QTimer();
	QObject::connect(this->_timer2, SIGNAL(timeout()), this, SLOT(timeout()));
	this->_timer2->start(10000);
	for (int i = 0; i < 10; i++){
		FtpDThread* uthread = new FtpDThread();
		QObject::connect(uthread, SIGNAL(dEnd(FtpDThread*)), this, SLOT(dthreadend(FtpDThread*)));
		QObject::connect(uthread, SIGNAL(dSuccess(QString)), this, SLOT(drunsuccess(QString)));
		QObject::connect(uthread, SIGNAL(dError(QString, QString)), this, SLOT(drunerror(QString, QString)));
		QObject::connect(uthread, SIGNAL(dProgress(QString, qint64, qint64)), this, SLOT(drunprogress(QString, qint64, qint64)));

		this->_freeDThreads.append(uthread);
	}
}
FtpDTool::~FtpDTool(){
	for (int i = 0; i < 10; i++){
		delete this->_freeDThreads[i];
	}
	delete this->_timer2;
}

/* *Summary: ���سɹ���slot*/
void FtpDTool::drunsuccess(QString id){
	emit this->dsuccess(id);
}
/* *Summary: �쳣����erro��slot*/
void FtpDTool::drunerror(QString id, QString message){
	emit this->derror(id, message);
}
/* *Summary: �����ϴ����ȵ�slot*/
void FtpDTool::drunprogress(QString id, qint64 bytesSent, qint64 bytesTotal){
	emit this->dprogress(id, bytesSent, bytesTotal);
}

/* *Summary: ��ʱ����*/
void FtpDTool::timeout(){
	QMutexLocker locker(&mutex);
	for (int i = this->_freeDThreads.length() - 1; i >= 0; i--){
		FtpDThread *dthread = this->_freeDThreads[i];
		if (this->_downloadfiles.length() > 0){
			dthread->download(this->_downloadfiles[0]);
			this->_downloadfiles.removeAt(0);
			this->_usedDThreads.append(dthread);
			this->_freeDThreads.removeAt(i);
		}
		else{
			break;
		}
	}
}

/* *Summary: �߳�end��ʱ��Ĵ���*/
void FtpDTool::dthreadend(FtpDThread *thread){
	QMutexLocker locker(&mutex);
	if (this->_downloadfiles.length() > 0){
		thread->download(this->_downloadfiles[0]);
		this->_downloadfiles.removeAt(0);
	}
	else
	{
		this->_freeDThreads.append(thread);
		this->_usedDThreads.removeOne(thread);
	}
}

void FtpDTool::getFileFormServer(QList<FtpDownloadFile> files)
{
	QMutexLocker locker(&mutex);
	FtpDownloadFile file;
	foreach(file, files) {
		this->_downloadfiles.append(file.getFtpDownloadOneFile());
	}
	qDebug() << "get OK";

}


