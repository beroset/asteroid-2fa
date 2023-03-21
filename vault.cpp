#include <vault.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <QVariant>
#include <sstream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <string>
#include <cstdlib>
#include <crypt.h>

using namespace std;

bool Vault::saveDatabase(QString password, QString db, int len){
   	Crypt crypto;
    string data = saveData(db,len);
    QJsonObject keyObj;
    
    if (password != ""){
    	
    	password = password.toUpper().toUtf8();
    	password = QString::fromStdString(crypto.scrypt(password.toStdString()));
    	string passwd = crypto.generateKey(password.toStdString());
		
		data = crypto.encrypt(data,passwd);
		data = int2hex(data);
		
		keyObj["salt"] = QString::fromStdString(int2hex(crypto.getSalt()));
		keyObj["tag"] = QString::fromStdString(int2hex(crypto.getTag()));
		keyObj["nonce"] = QString::fromStdString(int2hex(crypto.getNonce()));
	}else{
		keyObj["salt"] = "";
		keyObj["tag"] = "";
		keyObj["nonce"] = "";
	}
	
	
	
	QJsonObject rootObj;
	rootObj.insert("key",keyObj);
	rootObj.insert("db",QString::fromStdString(data));
	
	QJsonDocument doc(rootObj);
	QString jsonStr = doc.toJson();
	
	QFile sfile("/home/ceres/database.json");
	if(sfile.open(QIODevice::WriteOnly)){
		sfile.write(jsonStr.toUtf8());
		sfile.close(); 
	}
	return true;
}

QString Vault::readDatabase(QString password) {
	Crypt crypto;
	QFile jsonFile("/home/ceres/database.json");
	
 	jsonFile.open(QFile::ReadOnly);
	QJsonDocument d = QJsonDocument::fromJson(jsonFile.readAll());
	jsonFile.close();
	QJsonObject root = d.object();
	QVariantMap rootMap = root.toVariantMap();
	
	QVariantMap key = rootMap["key"].toMap();
	crypto.setSalt(hex2int(key["salt"].toString().toUtf8().toStdString())); 
	crypto.setTag(hex2int(key["tag"].toString().toUtf8().toStdString()));
	crypto.setNonce(hex2int(key["nonce"].toString().toUtf8().toStdString()));
	
	QString db = rootMap["db"].toString();
	db = db.toUtf8();
	string data = db.toStdString();
	
	if (password != ""){
    	password = password.toUpper().toUtf8();
    	password = QString::fromStdString(crypto.scrypt(password.toStdString()));
    	string passwd = crypto.generateKey(password.toStdString());
		
		data = hex2int(data);	
		data = crypto.decrypt(data,passwd);
	}
	return readData(QString::fromStdString(data));
	
}


QString Vault::readData(QString data){
	QJsonDocument d = QJsonDocument::fromJson(data.toLocal8Bit());
	QJsonArray db = d.array();
	data = "";
	for(int i=0;i<db.count();++i){
		QVariantMap o = db.at(i).toObject().toVariantMap();
		data += o["issuer"].toString() + ":";
		data += o["secret"].toString() + ";";
	}
    
	return data;
}

string Vault::saveData(QString data,int len){
	QStringList list = data.split(";");	
	
	QJsonArray database;
	for(int i=0;i<len;++i){
		QJsonObject db;
		QStringList tmp = list[i].split(":");
		db.insert("issuer",tmp[0]);
		db.insert("secret",tmp[1]);
        database.append(db);
    }
    
    QJsonDocument jsonDoc(database);
	QString tmp = jsonDoc.toJson();
	
	return tmp.toUtf8().toStdString();
}



string Vault::hex2int(string hex){
	string tmp = "";
	for(unsigned int i=0;i<hex.size();i+=2){
		tmp += (char)stoi(hex.substr(i,2),0,16);
	}
	return tmp;
}

string Vault::int2hex(string text){
	stringstream ss;
	ss.clear();
	for (auto str : text) 
	{
        ss << setfill('0') << setw(2) << hex << (int)str;
    }

	return ss.str();
}



