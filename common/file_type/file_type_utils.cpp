#include "file_type_utils.h"

#include <limits>
#include <QMap>
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <QTextCodec>


namespace FileTypeUtils {

FileTypeKey::FileTypeKey(const QString &key):key_(key)
{

}

FileTypeKey::~FileTypeKey()
{

}

bool FileTypeKey::operator<(const FileTypeKey &fileTypeKey) const
{
    int minSize = std::min(this->key_.length(),fileTypeKey.key_.length());
    qDebug() <<"operator < "<< QStringRef(&(this->key_),0,minSize) << QStringRef(&(fileTypeKey.key_),0,minSize);
    return QStringRef(&(this->key_),0,minSize) < QStringRef(&(fileTypeKey.key_),0,minSize);

}
QMap<FileTypeKey,QString> typemap = {
    {QString("0000000C6A502020"),"jp2"},
    {QString("0000002066747970"),"3gp"},
    {QString("0000001866747970"),"3gp5"},
    {QString("00000020667479704D3441"),"m4a"},
    {QString("00000100"),"ico"},
    {QString("00000100"),"spl"},
    {QString("000001BA"),"vob"},
    {QString("00000200"),"cur"},
    {QString("00000200"),"wb2"},
    {QString("0000020006040600"),"wk1"},
    {QString("00001A0000100400"),"wk3"},
    {QString("00001A0002100400"),"wk4"},
    {QString("00001A0002100400"),"wk5"},
    {QString("00001A00051004"),"123"},
    {QString("00004D4D585052"),"qxd"},
    {QString("010F0000"),"mdf"},
    {QString("0110"),"tr1"},
    {QString("01DA01010003"),"rgb"},
//    {QString("07"),"drw"},
    {QString("02647373"),"dss"},
    {QString("A90D000000000000"),"dat"},
//    {QString("03"),"db3"},
    {QString("03000000"),"qph"},
    {QString("80000020031204"),"adx"},
//    {QString("04"),"db4"},
    {QString("FFFE0000"),"n"},
    {QString("FFFE0000"),"a"},
    {QString("07534B46"),"skf"},
    {QString("0764743264647464"),"dtd"},
    {QString("D0CF11E0A1B11AE1"),"db"},
    {QString("0A050101"),"pcx"},
    {QString("0CED"),"mp"},
    {QString("D0CF11E0A1B11AE1"),"doc"},
    {QString("0E4E65726F49534F"),"nri"},
    {QString("FF00020004040554"),"wks"},
    {QString("1100000053434341"),"pf"},
    {QString("4E49544630"),"ntf"},
    {QString("4E45534D1A01"),"nsf"},
    {QString("41724301"),"arc"},
    {QString("5041434B"),"pak"},
    {QString("1A350100"),"eth"},
    {QString("1A45DFA393428288"),"mkv"},
    {QString("1D7D"),"ws"},
    {QString("1F8B08"),"gz"},
    {QString("1FA0"),"tar.z"},
    {QString("2112"),"ain"},
    {QString("213C617263683E0A"),"lib"},
    {QString("D0CF11E0A1B11AE1"),"msi"},
    {QString("4B444D"),"vmdk"},
    {QString("23204D6963726F73"),"dsp"},
    {QString("2321414D52"),"amr"},
    {QString("49536328"),"hdr"},
    {QString("24464C3240282329"),"sav"},
    {QString("C5D0D3C6"),"eps"},
    {QString("25504446"),"pdf"},
    {QString("25504446"),"fdf"},
    {QString("2854686973206669"),"hqx"},
    {QString("2A2A2A2020496E73"),"log"},
    {QString("2E524543"),"ivr"},
    {QString("2E524D46"),"rm"},
    {QString("2E524D46"),"rmvb"},
    {QString("2E7261FD00"),"ra"},
    {QString("646E732E"),"au"},
//    {QString("30"),"cat"},
    {QString("300000004C664C65"),"evt"},
    {QString("3026B2758E66CF11"),"asf"},
    {QString("3026B2758E66CF11"),"wma"},
    {QString("3026B2758E66CF11"),"wmv"},
    {QString("BE000000AB"),"wri"},
    {QString("377ABCAF271C"),"7z"},
    {QString("38425053"),"psd"},
    {QString("414376"),"sle"},
//    {QString("3C"),"asx"},
//    {QString("3C"),"xdr"},
    {QString("3C21646F63747970"),"dci"},
    {QString("3C3F786D6C2076657273696F6E3D"),"manifest"},
    {QString("3C3F786D6C2076657273696F6E3D22312E30223F3E"),"xml"},
    {QString("D0CF11E0A1B11AE1"),"msc"},
    {QString("3C4D616B65724669"),"fm"},
    {QString("56657273696F6E20"),"mif"},
    {QString("4C4E0200"),"gid"},
    {QString("4C4E0200"),"hlp"},
    {QString("41433130"),"dwg"},
    {QString("414D594F"),"syw"},
    {QString("414F4C494E444558"),"abi"},
    {QString("414F4C4442"),"aby"},
    {QString("414F4C2046656564"),"bag"},
    {QString("5000000020000000"),"idx"},
    {QString("414F4C494458"),"ind"},
    {QString("414F4C564D313030"),"pfc"},
    {QString("414F4C564D313030"),"org"},
    {QString("424547494E3A5643"),"vcf"},
    {QString("424C4932323351"),"bin"},
    {QString("424D"),"bmp"},
    {QString("424D"),"dib"},
    {QString("424F4F4B4D4F4249"),"prc"},
    {QString("425A68"),"bz2"},
    {QString("425A68"),"tar.bz2"},
    {QString("425A68"),"tbz2"},
    {QString("425A68"),"tb2"},
    {QString("43232B44A4434DA5"),"rtd"},
    {QString("434246494C45"),"cbd"},
    {QString("4344303031"),"iso"},
    {QString("434F4D2B"),"clb"},
    {QString("43505446494C45"),"cpt"},
    {QString("43525553482076"),"cru"},
    {QString("465753"),"swf"},
    {QString("436174616C6F6720"),"ctf"},
    {QString("444D5321"),"dms"},
    {QString("5245564E554D3A2C"),"adf"},
    {QString("445644"),"dvr"},
    {QString("445644"),"ifo"},
    {QString("52494646"),"cdr"},
    {QString("454E545259564344"),"vcd"},
    {QString("4550"),"mdi"},
    {QString("4C5646090D0AFF00"),"e01"},
    {QString("456C6646696C6500"),"evtx"},
    {QString("458600000600"),"qbb"},
    {QString("464158434F564552"),"cpe"},
    {QString("464C56"),"flv"},
    {QString("464F524D00"),"aiff"},
    {QString("582D"),"eml"},
    {QString("47494638"),"gif"},
    {QString("47504154"),"pat"},
    {QString("475832"),"gx2"},
    {QString("4848474231"),"sh3"},
    {QString("4D4D002B"),"tif"},
    {QString("4D4D002B"),"tiff"},
    {QString("494433"),"mp3"},
    {QString("49443303000000"),"koz"},
    {QString("49491A0000004845"),"crw"},
    {QString("4D534346"),"cab"},
    {QString("49544F4C49544C53"),"lit"},
    {QString("49545346"),"chi"},
    {QString("49545346"),"chm"},
    {QString("5F27A889"),"jar"},
    {QString("4A47040E000000"),"jg"},
    {QString("4B47425F61726368"),"kgb"},
    {QString("68490000"),"shd"},
    {QString("4C00000001140200"),"lnk"},
//    {QString("80"),"obj"},
    {QString("ACED000573720012"),"pdb"},
    {QString("4D41723000"),"mar"},
    {QString("504147454455"),"dmp"},
    {QString("4D444D5093A7"),"hdmp"},
    {QString("4D563243"),"mls"},
    {QString("4D4D4D440000"),"mmf"},
    {QString("4D52564E"),"nvram"},
    {QString("4D534346"),"ppz"},
    {QString("4D534346"),"snp"},
    {QString("4D53465402000100"),"tlb"},
    {QString("4D535F564F494345"),"dvf"},
    {QString("4D535F564F494345"),"msv"},
    {QString("4D546864"),"mid"},
    {QString("4D546864"),"midi"},
    {QString("4D56"),"dsn"},
//    {QString("EB"),"com"},
    {QString("4D5A"),"dll"},
    {QString("4D5A"),"drv"},
    {QString("4D5A"),"exe"},
    {QString("4D5A"),"pif"},
    {QString("4D5A"),"qts"},
    {QString("4D5A"),"qtx"},
    {QString("FFFFFFFF"),"sys"},
    {QString("4D5A"),"acm"},
    {QString("4D5A900003000000"),"ax"},
    {QString("DCDC"),"cpl"},
    {QString("4D5A"),"fon"},
    {QString("4D5A"),"ocx"},
    {QString("4D5A"),"olb"},
    {QString("4D5A"),"scr"},
    {QString("4D5A"),"vbx"},
    {QString("4D5A"),"vxd"},
    {QString("4D5A"),"386"},
    {QString("4D5A900003000000"),"api"},
    {QString("76323030332E3130"),"flt"},
    {QString("4D5A90000300000004000000FFFF"),"zap"},
    {QString("4D6963726F736F66742056697375616C"),"sln"},
    {QString("4E422A00"),"jnt"},
    {QString("4E422A00"),"jtp"},
    {QString("4E616D653A20"),"cod"},
    {QString("4F504C4461746162"),"dbf"},
    {QString("4F67675300020000"),"oga"},
    {QString("4F67675300020000"),"ogg"},
    {QString("4F67675300020000"),"ogv"},
    {QString("4F67675300020000"),"ogx"},
    {QString("4F7B"),"dw4"},
    {QString("50350A"),"pgm"},
    {QString("504158"),"pax"},
    {QString("504750644D41494E"),"pgd"},
    {QString("EB3C902A"),"img"},
    {QString("504B0304140000"),"zip"},
    {QString("504B030414000600"),"docx"},
    {QString("504B030414000600"),"pptx"},
    {QString("504B030414000600"),"xlsx"},
    {QString("504B0304"),"kwd"},
    {QString("504B0304"),"odt"},
    {QString("504B0304"),"odp"},
    {QString("504B0304"),"ott"},
    {QString("504B0304"),"sxc"},
    {QString("504B0304"),"sxd"},
    {QString("504B0304"),"sxi"},
    {QString("504B0304"),"sxw"},
    {QString("504B0304"),"wmz"},
    {QString("504B0304"),"xpi"},
    {QString("504B0304"),"xps"},
    {QString("5850434F4D0A5479"),"xpt"},
    {QString("504D4343"),"grp"},
    {QString("514649"),"qemu"},
    {QString("5157205665722E20"),"abd"},
    {QString("5157205665722E20"),"qsd"},
    {QString("FFFE"),"reg"},
    {QString("52454745444954"),"sud"},
    {QString("52494646"),"ani"},
    {QString("52494646"),"cmx"},
    {QString("52494646"),"ds4"},
    {QString("52494646"),"4xm"},
    {QString("52494646"),"avi"},
    {QString("52494646"),"cda"},
    {QString("52494646"),"qcp"},
    {QString("52494646"),"rmi"},
    {QString("52494646"),"wav"},
    {QString("58435000"),"cap"},
    {QString("526172211A0700"),"rar"},
    {QString("5343486C"),"ast"},
    {QString("53484F57"),"shw"},
    {QString("FF464F4E54"),"cpi"},
    {QString("5374756666497420"),"sit"},
    {QString("534D415254445257"),"sdr"},
    {QString("53514C4F434F4E56"),"cnv"},
    {QString("B5A2B0B3B3B0A5B5"),"cal"},
    {QString("E310000100000000"),"info"},
    {QString("55434558"),"uce"},
    {QString("554641C6D2C1"),"ufa"},
    {QString("564350434830"),"pch"},
    {QString("56455253494F4E20"),"ctl"},
    {QString("575332303030"),"ws2"},
    {QString("576F726450726F"),"lwp"},
    {QString("5854"),"bdr"},
    {QString("5A4F4F20"),"zoo"},
    {QString("5B47656E6572616C"),"ecf"},
    {QString("5B4D535643"),"vcw"},
    {QString("5B50686F6E655D"),"dun"},
    {QString("5B7665725D"),"sam"},
    {QString("5B57696E646F7773"),"cpx"},
    {QString("5B666C7473696D2E"),"cfg"},
    {QString("5F434153455F"),"cas"},
    {QString("5F434153455F"),"cbk"},
    {QString("60EA"),"arj"},
    {QString("636F6E6563746978"),"vhd"},
    {QString("6375736800000002"),"csh"},
    {QString("64000000"),"p10"},
    {QString("6465780A30303900"),"dex"},
    {QString("64737766696C65"),"dsw"},
    {QString("664C614300000022"),"flac"},
    {QString("6C33336C"),"dbb"},
    {QString("72696666"),"acd"},
    {QString("727473703A2F2F"),"ram"},
//    {QString("78"),"dmg"},
    {QString("7B0D0A6F20"),"lgc"},
    {QString("7B0D0A6F20"),"lgd"},
    {QString("7B5C707769"),"pwi"},
    {QString("7B5C72746631"),"rtf"},
    {QString("7E424B00"),"psp"},
    {QString("9CCBCB8D1375D211"),"wab"},
    {QString("81CDAB"),"wpf"},
    {QString("8A0109000000E108"),"aw"},
    {QString("91334846"),"hap"},
    {QString("9501"),"skr"},
//    {QString("99"),"gpg"},
    {QString("9901"),"pkr"},
    {QString("AC9EBD8F0000"),"qdf"},
    {QString("E3828596"),"pwl"},
    {QString("B168DE3A"),"dcx"},
    {QString("B46E6844"),"tib"},
    {QString("C3ABCDAB"),"acs"},
    {QString("C8007900"),"lbk"},
    {QString("CAFEBABE"),"class"},
    {QString("CFAD12FE"),"dbx"},
    {QString("D0CF11E0A1B11AE1"),"dot"},
    {QString("D0CF11E0A1B11AE1"),"pps"},
    {QString("D0CF11E0A1B11AE1"),"ppt"},
    {QString("D0CF11E0A1B11AE1"),"xla"},
    {QString("D0CF11E0A1B11AE1"),"xls"},
    {QString("D0CF11E0A1B11AE1"),"wiz"},
    {QString("D0CF11E0A1B11AE1"),"ac_"},
    {QString("D0CF11E0A1B11AE1"),"adp"},
    {QString("D0CF11E0A1B11AE1"),"apr"},
    {QString("D0CF11E0A1B11AE1"),"mtw"},
    {QString("D0CF11E0A1B11AE1"),"opt"},
    {QString("D0CF11E0A1B11AE1"),"pub"},
    {QString("D0CF11E0A1B11AE1"),"rvt"},
    {QString("D0CF11E0A1B11AE1"),"sou"},
    {QString("D0CF11E0A1B11AE1"),"spo"},
    {QString("D0CF11E0A1B11AE1"),"vsd"},
    {QString("D0CF11E0A1B11AE1"),"wps"},
    {QString("D20A0000"),"ftr"},
    {QString("D42A"),"arl"},
    {QString("D42A"),"aut"},
    {QString("D7CDC69A"),"wmf"},
    {QString("DCFE"),"efx"},
    {QString("E4525C7B8CD8A74D"),"one"},
    {QString("EDABEEDB"),"rpm"},
    {QString("FEEF"),"gho"},
    {QString("FEEF"),"ghs"},
    {QString("FF575043"),"wp"},
    {QString("FF575043"),"wpd"},
    {QString("FF575043"),"wpg"},
    {QString("FF575043"),"wpp"},
    {QString("FF575043"),"wp5"},
    {QString("FF575043"),"wp6"},
    {QString("FFD8FF"),"jfif"},
    {QString("FFD8FF"),"jpe"},
    {QString("FFD8FF"),"jpeg"},
    {QString("FFD8FF"),"jpg"},
    {QString("89504E47"),"png"},
    {QString("FFFE23006C006900"),"mof"},
    {QString("504B03040A"),"ipa"}
};



FileType findFileType(const QString &hexStr)
{
    qDebug() << "findFileType hexStr:" << hexStr;
    QString type;
    if(!hexStr.isEmpty()){
        type = typemap.value(FileTypeKey(hexStr));
    }
    FileType ft = { type,"" };
    return ft;
}

bool is_str_gbk(const char* str,int len)
{
    unsigned int nBytes = 0;//GBK可用1-2个字节编码,中文两个 ,英文一个
    unsigned char chr = *str;
    bool bAllAscii = true; //如果全部都是ASCII,
    for (unsigned int i = 0; i<len; ++i){
        chr = *(str + i);
        if ((chr & 0x80) != 0 && nBytes == 0){// 判断是否ASCII编码,如果不是,说明有可能是GBK
            bAllAscii = false;
        }
        if (nBytes == 0) {
            if (chr >= 0x80) {
                if (chr >= 0x81 && chr <= 0xFE){
                    nBytes = +2;
                }
                else{
                    return false;
                }
                nBytes--;
            }
        }
        else{
            if (chr < 0x40 || chr>0xFE){
                return false;
            }
            nBytes--;
        }//else end
    }
    if (nBytes != 0)  {     //违返规则
        return false;
    }
    if (bAllAscii){ //如果全部都是ASCII, 也是GBK
        return true;
    }
    return true;
}

bool is_str_utf8(const char* str,int len)
{
    unsigned int nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; i<len; ++i){
        chr = *(str + i);
        //判断是否ASCII编码,如果不是,说明有可能是UTF8,ASCII用7位编码,最高位标记为0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0){
            bAllAscii = false;
        }
        if (nBytes == 0) {
            //如果不是ASCII码,应该是多字节符,计算字节数
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD){
                    nBytes = 6;
                }
                else if (chr >= 0xF8){
                    nBytes = 5;
                }
                else if (chr >= 0xF0){
                    nBytes = 4;
                }
                else if (chr >= 0xE0){
                    nBytes = 3;
                }
                else if (chr >= 0xC0){
                    nBytes = 2;
                }
                else{
                    return false;
                }
                nBytes--;
            }
        }
        else{
            //多字节符的非首字节,应为 10xxxxxx
            if ((chr & 0xC0) != 0x80){
                return false;
            }
            //减到为零为止
            nBytes--;
        }
    }
    //违返UTF8编码规则
    if (nBytes != 0)  {
        return false;
    }
    if (bAllAscii){ //如果全部都是ASCII, 也是UTF8
        return true;
    }
    return true;
}

FileType findFileTypeByFilePath(const QString &filePath)
{
    FileType fileType;
    {
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)) {
            return fileType;
        }
        QDataStream in(&file);
        char data[16]={0};
        int len = in.readRawData(data,16);
        QString key;
        for(int i=0;i<len;i++){
            key.append(QString::number((uint)data[i]&0x000000ff,16).toUpper());
        }
        fileType =  findFileType(key);
        file.close();
    }

    if(fileType.type.isEmpty()){
        QFile file(filePath);
        if(!file.open(QIODevice::ReadOnly)) {
            return fileType;
        }
        QByteArray data = file.readAll();
        if(is_str_utf8(data.data(),data.length())){
            fileType.characterSet="utf-8";
            fileType.type="text";
        }else if(is_str_gbk(data.data(),data.length())){
            fileType.characterSet="gbk";
            fileType.type="text";
        }
    }

    qDebug() << "findFileType:" << fileType.type <<" char:"<< fileType.characterSet;

}

}
