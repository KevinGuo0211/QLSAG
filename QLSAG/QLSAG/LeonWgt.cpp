#include "LeonWgt.h"

#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

#define WIN_WIDTH 600
#define WIN_HEIGHT 600

// FTPĬ�϶˿ں�
#define FTP_PORT 21

QLSAG::QLSAG(QWidget *parent)
	: QWidget(parent)
	, m_pfftw(NULL)
{
	InitUI();
}

QLSAG::~QLSAG()
{

}


void QLSAG::InitUI()
{
	ui.setupUi(this);
	resize(WIN_WIDTH, WIN_HEIGHT);

	SiganlConnect();

	InitData();

	if (NULL == m_pfftw)
	{
		m_pfftw = new FFTW;
	}
	QHBoxLayout *hLayout = new QHBoxLayout(ui.m_pFFTWgt);
	hLayout->setContentsMargins(0, 0, 0, 0);
	hLayout->addWidget(m_pfftw);
	ui.m_pFFTWgt->setLayout(hLayout);
}

void QLSAG::SiganlConnect()
{
	QObject::connect(ui.m_pDownloadBtn, SIGNAL(clicked()), this, SLOT(OnDownloadBtn()));
	QObject::connect(ui.m_pServerFileComBox, SIGNAL(activated(const QString &)), 
		this, SLOT(OnAddDownloadFileName(const QString &)));
	QObject::connect(ui.m_pLocalPathBtn, SIGNAL(clicked()), this, SLOT(OnSelectLocalPath()));

	QObject::connect(FtpDTool::getInstance(), SIGNAL(dsuccess(QString)),
		this, SLOT(download_success(QString)));
	QObject::connect(FtpDTool::getInstance(), SIGNAL(derror(QString, QString)),
		this, SLOT(download_error(QString, QString)));
	QObject::connect(FtpDTool::getInstance(), SIGNAL(dprogress(QString, qint64, qint64)),
		this, SLOT(download_progress(QString, qint64, qint64)));

	QObject::connect(this, SIGNAL(SignalSuccess(const QString&, const QString&, bool)),
		this, SLOT(OnSuccess(const QString&, const QString&, bool)));
	QObject::connect(this, SIGNAL(SignalFailure(const QString&, const QString&, bool)),
		this, SLOT(OnFailure(const QString&, const QString&, bool)));
	QObject::connect(this, SIGNAL(Signal_download_progress(const QString&, qint64, qint64)),
		this, SLOT(OnUploadProgress(const QString&, qint64, qint64)));

	QObject::connect(this, SIGNAL(Signal_download_sucess(const QString&, const QString&)), 
		this, SLOT(OnParseDownLoadFile(const QString&, const QString&)));

	QObject::connect(ui.m_pfftwBtn, SIGNAL(clicked()), this, SLOT(OnFFTWBtn()));

}

void QLSAG::InitData()
{
	m_strSavePath = "";
	m_strFilePath = "../FtpDownload";

	//ui.m_pServerIpEdit->setInputMask("000.000.0.000");
	ui.m_pServerIpEdit->setText("127.0.0.1");
	ui.m_pUserNameEdit->setText("target");
	ui.m_pUserPasswdEdit->setText("target");
	ui.m_pServerPathEdit->setText("/Tffs/");
	ui.m_pLocalPathEdit->setText(m_strFilePath);
	ui.m_pServerFileComBox->addItem("GooseSend.iec");
	ui.m_pServerFileComBox->addItem("GooseRecv.iec");
	ui.m_pServerFileComBox->addItem("SMVIEC61850_9_2.iec");
	ui.m_pServerFileComBox->addItem("SMVIEC60044_7_8.iec");
	ui.m_pServerFileComBox->addItem("STM32F429.pdf");
	ui.m_pProgressBar->setValue(0);

	ui.m_pUserNameEdit->setPlaceholderText(QStringLiteral("�û���"));
	ui.m_pUserPasswdEdit->setPlaceholderText(QStringLiteral("��  ��"));
	ui.m_pServerPathEdit->setPlaceholderText(QStringLiteral("��ʽ��/Tffs/"));
	ui.m_pServerIpEdit->setPlaceholderText(QStringLiteral("��ʽ��127.0.0.1"));
	//ui.m_pServerFileComBox->addItem("STM32F429.pdf");
}


void QLSAG::OnAddDownloadFileName(const QString &strFileName)
{
	ui.m_pProgressBar->setValue(0);
	QComboBox *cBox = qobject_cast<QComboBox *>(sender());
	QStringList strLst;
	int iCount = cBox->count();
	for (int iIndex = 0; iIndex < iCount; ++iIndex)
	{
		QString strText = cBox->itemText(iIndex);
		strLst.append(strText);
	}

	if (!strLst.contains(strFileName))
	{
		cBox->addItem(strFileName);
	}
	
}


void QLSAG::RecvFileFromServer(QMap<QString, QStringList> &mapIpFilePath)
{
	QList<FtpDownloadFile> ftpFileLst;
	QMap<QString, QStringList>::iterator itFilePath;
	for (itFilePath = mapIpFilePath.begin(); itFilePath != mapIpFilePath.end(); ++itFilePath)
	{
		// ������IP
		QString strIp = itFilePath.key();
		// �����ص��ļ�
		QList<QString> strFileLst = itFilePath.value();

		FtpDownloadFile ftpFile;
		ftpFile.setIP(strIp);
		ftpFile.setPort(FTP_PORT);
		ftpFile.setUser(ui.m_pUserNameEdit->text());
		ftpFile.setPwd(ui.m_pUserPasswdEdit->text());
		ftpFile.setServerPath(ui.m_pServerPathEdit->text());
		ftpFile.setFiles(strFileLst);

		ftpFileLst.append(ftpFile);

		// �����ļ�
		this->_downloadfiles.append(ftpFile.getFtpDownloadOneFile());


	}
	if (ftpFileLst.size() > 0)
	{
		FtpDTool::getInstance()->getFileFormServer(ftpFileLst);
	}
}


void QLSAG::OnSelectLocalPath()
{
	// ��ȡѡ������Ŀ¼
	m_strSavePath = GetSaveFilePath(m_strFilePath, m_strSavePath);
	m_strFilePath = m_strSavePath;
	ui.m_pLocalPathEdit->setText(m_strSavePath);
}


void QLSAG::OnDownloadBtn()
{
	QString strFileName;
	ui.m_pDownloadBtn->setEnabled(false);

	m_strSavePath = ui.m_pLocalPathEdit->text();
	strFileName = m_strSavePath + "/" + ui.m_pServerFileComBox->currentText();

	QStringList strLst2;
	strLst2 << strFileName;

	// �ж��ļ��Ƿ����
	QFileInfo info(strFileName);
	if (!info.isFile())
	{
		// ����
		QMap<QString, QStringList> mapIpFilePath2;
		mapIpFilePath2.insert(ui.m_pServerIpEdit->text(), strLst2);
		RecvFileFromServer(mapIpFilePath2); 
	}
	else
	{
		// �ļ��Ѵ�����Ϣ��ʾ
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("��ʾ"));
		msgBox.setText(QString::fromLocal8Bit("�ļ���") + strFileName + "\n"
			+ QString::fromLocal8Bit("�Ѵ��ڣ�"));
		QPushButton *OkBtn = msgBox.addButton(QString::fromLocal8Bit("ȷ��"), QMessageBox::AcceptRole);
		//QPushButton *CancelBtn = msgBox.addButton(QString::fromLocal8Bit("ȡ��"), QMessageBox::RejectRole);
		msgBox.exec();

		if (msgBox.clickedButton() == OkBtn)
		{
			msgBox.close();
			ui.m_pDownloadBtn->setEnabled(true);
		}
		
	}
	
}

QString QLSAG::GetSaveFilePath(QString strFilePath, QString strSavePath)
{
	QDir* dir = new QDir;
	if (!(dir->exists(strFilePath)))
	{
		dir->mkdir(strFilePath);
	}

	if (strFilePath.isEmpty())
	{
		strSavePath = QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ������Ŀ¼"),
			QDir::currentPath(),
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
	}
	else
	{
		strSavePath = QFileDialog::getExistingDirectory(this,
			QStringLiteral("ѡ������Ŀ¼"),
			strFilePath,
			QFileDialog::ShowDirsOnly
			| QFileDialog::DontResolveSymlinks);
	}

	qDebug() << "------strSavePath ------>" << strSavePath;

	return strSavePath;
}


void QLSAG::download_success(QString id)
{
	FtpDownloadOneFile f(id);
	FtpDownloadOneFile file = this->_downloadfiles[this->_downloadfiles.indexOf(f)];
	QString strMsg = "IP:[" + file.getIP() + "]; File:[" + file.getFile() + "] Upload Success \r\n";
	qPrintable(strMsg);

	emit SignalSuccess(file.getIP(), file.getFile(), true);

}

void QLSAG::download_error(QString id, QString message)
{
	FtpDownloadOneFile f(id);
	FtpDownloadOneFile file = this->_downloadfiles[this->_downloadfiles.indexOf(f)];
	QString strMsg = "IP:[" + file.getIP() + "]; File:[" + file.getFile() + "] Upload Error Code[" + message + "] \r\n";
	qPrintable(strMsg);

	emit SignalFailure(file.getIP(), file.getFile(), false);

}

void QLSAG::download_progress(QString id, qint64 bytesSent, qint64 bytesTotal)
{
	QString strMsg;
	FtpDownloadOneFile f(id);
	FtpDownloadOneFile file = this->_downloadfiles[this->_downloadfiles.indexOf(f)];
	if (bytesTotal != 0){
		strMsg = "IP:[" + file.getIP() + "]; File:[" + file.getFile() + "] Upload Progress \r\n Send:"
			+ QString::number(bytesSent) + "byte; Total:"
			+ QString::number(bytesTotal) + "byte; +++++++ "
			+ QString::number(bytesSent * 100 / bytesTotal, 'f', 2) + "% \r\n";
		int prosses = bytesSent * 100 / bytesTotal;
		qPrintable(strMsg);

		emit Signal_download_progress(file.getIP(), bytesSent, bytesTotal);
	}
	else{

		strMsg = "IP:[" + file.getIP() + "]; File:[" + file.getFile() + "] Upload Progress \r\n Send:"
			+ QString::number(bytesSent) + "byte; Total:"
			+ QString::number(bytesTotal) + "byte; +++++++ "
			+ "0" + "% \r\n";
		qPrintable(strMsg);

		emit Signal_download_progress(file.getIP(), bytesSent, bytesTotal);
	}
}

void QLSAG::OnSuccess(const QString& strIp, const QString& strFile, bool bFlag)
{
	// �ź������ļ�������
	QFileInfo file(strFile);
	QString strFileName = file.fileName();
	emit Signal_download_sucess(strIp, strFileName);
}

void QLSAG::OnFailure(const QString& strIp, const QString& strFile, bool bFlag)
{
	// ������Ϣ��ʾ
	QMessageBox msgBox;
	msgBox.setWindowTitle(QString::fromLocal8Bit("ʧ��"));
	msgBox.setText(QString::fromLocal8Bit("������IP��") + strIp + "\n"
		+ QString::fromLocal8Bit("�ļ���") + strFile + "\n"
		+ QString::fromLocal8Bit("����ʧ�ܣ�"));
	QPushButton *OkBtn = msgBox.addButton(QString::fromLocal8Bit("ȷ��"), QMessageBox::AcceptRole);
	//QPushButton *CancelBtn = msgBox.addButton(QString::fromLocal8Bit("ȡ��"), QMessageBox::RejectRole);
	msgBox.exec();

	if (msgBox.clickedButton() == OkBtn)
	{
		ui.m_pProgressBar->setValue(0);
		ui.m_pDownloadBtn->setEnabled(true);
		msgBox.close();
	}
}

void QLSAG::OnUploadProgress(const QString& id, qint64 bytesSent, qint64 bytesTotal)
{
	ui.m_pProgressBar->setMinimum(0);
	ui.m_pProgressBar->setMaximum(bytesTotal);
	ui.m_pProgressBar->setValue(bytesSent);
}

void QLSAG::OnParseDownLoadFile(const QString& strIp, const QString& strFileName)
{
	ui.m_pDownloadBtn->setEnabled(true);
	// �����ļ�
	// @todo
	QString strFile = strFileName;
	int  i = 0;
}

void QLSAG::OnFFTWBtn()
{
	
	m_pfftw->InitData();

}






