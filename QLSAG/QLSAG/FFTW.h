/************************************************************************
*   �ļ����ƣ�FFTW.h
*   ��д��Ա��LHH
*   �������ڣ�2018/01/09
*   ����������FFTW����Ҷ�任
*   ��    ע��
*   �޸�������
************************************************************************/
#ifndef FFTW_H
#define FFTW_H

#include "fftw3.h"
#include <QWidget>
#include "ui_FFTW.h"

class FFTW : public QWidget
{
	Q_OBJECT

public:
	FFTW(QWidget *parent = 0);
	~FFTW();

	/**
	* @author by lhh
	* @brief ��ʼ������
	* @param ��
	* @return void
	*/
	void InitUI();

	/**
	* @author by lhh
	* @brief ��ʼ������
	* @param ��
	* @return void
	*/
	void InitData();

	/**
	* @author by lhh
	* @brief �źŲ�����
	* @param ��
	* @return void
	*/
	void SiganlConnect();

private:
	Ui::FFTW ui;
};

#endif // FFTW_H
