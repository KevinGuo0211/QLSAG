/************************************************************************
*   �ļ����ƣ�LeonWgt.h
*   ��д��Ա��LHH
*   �������ڣ�2018/01/09
*   ����������Ftp���ؽ��漰Ƶ�׷�������
*   ��    ע��
*   �޸�������
************************************************************************/

#ifndef LEONWGT_H
#define LEONWGT_H

#include "../FtpManager/FtpManager/FtpDownloadFile.h"
#include "../FtpManager/FtpManager/ftptool.h"

#include "FFTW.h"

#include <QtWidgets/QWidget>
#include "ui_LeonWgt.h"

class QLSAG : public QWidget
{
	Q_OBJECT

public:
	QLSAG(QWidget *parent = 0);
	~QLSAG();

	/**
	* @author by lhh
	* @brief ��ʼ������
	* @param ��
	* @return void
	*/
	void InitUI();

	/**
	* @author by lhh
	* @brief �źŲ�����
	* @param ��
	* @return void
	*/
	void SiganlConnect();

	/**
	* @author by lhh
	* @brief ��ʼ������
	* @param ��
	* @return void
	*/
	void InitData();

	/*@author by lhh
	* @brief �������ص��ļ�
	* @param IP��֮��ص��ļ�����·��
	* @return void
	*/
	void RecvFileFromServer(QMap<QString, QStringList> &mapIpFilePath);

	/**
	* @author by lhh
	* @brief ��ȡ�����ļ�·��
	* @param �ļ�·��
	* @param ����·��
	* @return void
	*/
	QString GetSaveFilePath(QString strFilePath, QString strSavePath);


Q_SIGNALS:
	/**
	* @author by lhh
	* @brief ���ͳɹ����ź�
	* @param Ŀ��IP
	* @param ѡ����ļ�
	* @return void
	*/
	void SignalSuccess(const QString& strIp, const QString& strFiles, bool bFlag);

	/**
	* @author by lhh
	* @brief ����ʧ�ܵ��ź�
	* @param Ŀ��IP
	* @param ѡ����ļ�
	* @return void
	*/
	void SignalFailure(const QString& strIp, const QString& strFiles, bool bFlag);

	/**
	* @author by lhh
	* @brief �����ļ����ȵ��ź�
	* @param ��ǰ�豸IP
	* @param �ѷ����ļ����ֽ���
	* @param �ļ����ֽ���
	* @return void
	*/
	void Signal_download_progress(const QString& id, qint64 bytesSent, qint64 bytesTotal);

	/**
	* @author by lhh
	* @brief �ļ����سɹ����͵��ź�
	* @param ip��port��key
	* @return void
	*/
	void Signal_download_sucess(const QString& ip, const QString& strFileName);

private slots:
	/**
	* @author by lhh
	* @brief ��������ļ�
	* @param �ļ� 
	* @return void
	*/
	void OnAddDownloadFileName(const QString &strFileName);

	/**
	* @author by lhh
	* @brief ѡ�񱣴��ļ�·��
	* @param ��
	* @return void
	*/
	void OnSelectLocalPath();
	
	/**
	* @author by lhh
	* @brief �����ļ�
	* @param ��
	* @return void
	*/
	void OnDownloadBtn();

	void download_success(QString id);//�ɹ����ļ�����·����
	void download_error(QString id, QString message);//�����ļ�����·����������Ϣ��
	void download_progress(QString id, qint64 bytesSent, qint64 bytesTotal);//���ȣ��ϴ���Сbyte���ܴ�Сbyte��

	/**
	* @author by lhh
	* @brief ���ͳɹ��Ĳۺ���
	* @param Ŀ��IP
	* @param ѡ����ļ�
	* @return void
	*/
	void OnSuccess(const QString& strIp, const QString& strFile, bool bFlag);

	/**
	* @author by lhh
	* @brief ����ʧ�ܵĲۺ���
	* @param Ŀ��IP
	* @param ѡ����ļ�
	* @return void
	*/
	void OnFailure(const QString& strIp, const QString& strFile, bool bFlag);

	/**
	* @author by lhh
	* @brief ���շ����ļ����ȵĲۺ���
	* @param ��ǰ�豸IP
	* @param �ѷ����ļ����ֽ���
	* @param �ļ����ֽ���
	* @return void
	*/
	void OnUploadProgress(const QString& id, qint64 bytesSent, qint64 bytesTotal);

	/**
	* @author by lhh
	* @brief �����ļ�
	* @param ��������ַ
	* @param ���յ��ļ�
	* @return void
	*/
	void OnParseDownLoadFile(const QString& strIp, const QString& strFileName);

	/**
	* @author by lhh
	* @brief ����Ҷ�任
	* @param ��
	* @return void
	*/
	void OnFFTWBtn();

private:
	QList<FtpUploadOneFile> _uploadfiles;
	QList<FtpDownloadOneFile> _downloadfiles;

private:
	Ui::QLSAGClass ui;

	QString m_strFilePath;
	QString m_strSavePath;

	FFTW *m_pfftw;

	
};

#endif // LEONWGT_H
