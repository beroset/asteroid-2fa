#include "qrcode.h"
#include <string>
#include <vector>
#include "qrcode/qrcodegen.h"
#include <sstream>
#include <QImage>
#include <QFile>
#include "QZXing.h"

using qrcodegen::QrCode;
using qrcodegen::QrSegment;
using namespace std;

void QRCode::generateQRCode() {
	QFile rfile("/home/ceres/database.json");
    if (!rfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if(rfile.atEnd()) {
    	rfile.close();
    	return;    
    }	
	QByteArray str = rfile.readAll();
	rfile.close();
	QString data = QString::fromUtf8(str);
	data = data.simplified();
	QrCode qr0 = QrCode::encodeText(data.toStdString().c_str(), QrCode::Ecc::LOW);
	string svg = toSvgString(qr0, 4);  
	data = QString::fromStdString(svg);
	
	QFile sfile("/home/ceres/export.svg");
	if(sfile.open(QIODevice::WriteOnly)){
		sfile.write(data.toUtf8());
		sfile.close(); 
	}
	
}

void QRCode::generateQRCodeTest() {
	QFile rfile("/home/ceres/database.json");
    if (!rfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    if(rfile.atEnd()) {
    	rfile.close();
    	return;    
    }	
	QByteArray str = rfile.readAll();
	rfile.close();
	QString data = QString::fromUtf8(str);
	data = data.simplified();
    QImage barcode = QZXing::encodeData(data);
	//QImage barcode = QZXing::encodeData(data, QZXing::EncoderFormat_QR_CODE,
	//							QSize(240, 240), QZXing::EncodeErrorCorrectionLevel_L);
	barcode.save("/home/ceres/Pictures/export.jpg");
}



string QRCode::toSvgString(const QrCode &qr, int border) {
	
	/*
	if (border < 0)
		throw std::domain_error("Border must be non-negative");
	if (border > INT_MAX / 2 || border * 2 > INT_MAX - qr.getSize())
		throw overflow_error("Border too large");
	*/
	std::stringstream sb;
	sb << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	sb << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n";
	sb << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" viewBox=\"0 0 ";
	sb << (qr.getSize() + border * 2) << " " << (qr.getSize() + border * 2) << "\" stroke=\"none\">\n";
	sb << "\t<rect width=\"100%\" height=\"100%\" fill=\"#FFFFFF\"/>\n";
	sb << "\t<path d=\"";
	for (int y = 0; y < qr.getSize(); y++) {
		for (int x = 0; x < qr.getSize(); x++) {
			if (qr.getModule(x, y)) {
				if (x != 0 || y != 0)
					sb << " ";
				sb << "M" << (x + border) << "," << (y + border) << "h1v1h-1z";
			}
		}
	}
	sb << "\" fill=\"#000000\"/>\n";
	sb << "</svg>\n";
	return sb.str();
}
