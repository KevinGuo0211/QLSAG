#include "FFTW.h"
#include <QDebug>

#define PI 3.1415926

FFTW::FFTW(QWidget *parent)
	: QWidget(parent)
{
	InitUI();
}

FFTW::~FFTW()
{

}

void FFTW::InitUI()
{
	ui.setupUi(this);

	SiganlConnect();

	//InitData();
}


void FFTW::SiganlConnect()
{

}


void FFTW::InitData()
{

	//qreal len;
	int len = 8;
	double *in = NULL;
	// ���Ҫʹ��float�汾,��������float�汾��lib��,Ȼ����fftw�������f��׺����.  
	fftw_complex *out = NULL;// fftwf_complex --> ��Ϊfloat�汾  
	fftw_plan p;
	in = (double *)fftw_malloc(sizeof(double)* len);
	out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex)* len);
	double dx = 1.0 / len;

	QString strZIn = NULL;
	// ���봿ʵ��  
	for (int i = 0; i < len; i++)
	{
		in[i] = sin(2 * PI * dx*i) + sin(4 * PI * dx*i);

		//printf("%.2f ", in[i]);
		QString strIn = QString::number(in[i], 'f', 2);
		strZIn += " " + strIn + " ";
	}
	qDebug() << strZIn << " ";
	ui.textEdit->append(strZIn);

	// ����Ҷ�任  
	p = fftw_plan_dft_r2c_1d(len, in, out, FFTW_ESTIMATE);
	fftw_execute(p);

	QString strZOut = NULL;
	// ���������  
	for (int i = 0; i < len; i++)
	{
		float len = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
		//printf("%.2f ", len);
		QString strOut = QString::number(len, 'f', 2);

		strZOut += " " + strOut + " ";
		
	}
	qDebug() << strZOut << " ";
	ui.textEdit->append(strZOut);

	// �ͷ���Դ  
	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	//system("pause");
}
