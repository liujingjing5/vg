#include "code_editor.h"
#include <Qsci/qscilexeravs.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercoffeescript.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexercustom.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerjson.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexermarkdown.h>
#include <Qsci/qscilexermatlab.h>
#include <Qsci/qscilexeroctave.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpo.h>
#include <Qsci/qscilexerpostscript.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerproperties.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerspice.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertcl.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

#include <QMessageBox>
#include <QtGlobal>
#include <QTime>
#include <QRegularExpression>
#include <QDebug>
#include "common/utils.h"

namespace CodeEditEnum {
    const static char* type = "type";
    const static char*  dirty = "dirty";
    const static char*  path = "path";
}


QsciLexer* getLexer(const QString& fileName,const QString& extName,QObject*parent=0){
    qDebug() << "fileName:" <<fileName <<" extName:"<<extName;
    if(extName == "avs"){
        return new QsciLexerAVS(parent);
    }else if(extName == "bash" || extName == "sh"){
        return new QsciLexerBash(parent);
    }else if(extName == "batch"){
        return new QsciLexerBatch(parent);
    }else if(fileName == "cmakelists.txt"){
        return new QsciLexerCMake(parent);
    }else if(extName == "coffee"){
        return new QsciLexerCoffeeScript(parent);
    }else if(extName == "h" || extName == "hpp"|| extName == "c"||extName == "cpp"||extName == "cc"||extName == "cxx"||extName == "m"||extName == "mm"){
        return new QsciLexerCPP(parent);
    }else if(extName == "cs"){
        return new QsciLexerCSharp(parent);
    }else if(extName == "css"){
        return new QsciLexerCSS(parent);
    }else if(extName == "d"){
        return new QsciLexerD(parent);
    }else if(extName == "diff"){
        return new QsciLexerDiff(parent);
    }else if(extName == "fortran"){
        return new QsciLexerFortran(parent);
    }else if(extName == "fortran77"){
        return new QsciLexerFortran77(parent);
    }else if(extName == "html"||extName == "htm"||extName == "xhtml"){
        return new QsciLexerHTML(parent);
    }else if(extName == "idl"){
        return new QsciLexerIDL(parent);
    }else if(extName == "java"){
        return new QsciLexerJava(parent);
    }else if(extName == "js"){
        return new QsciLexerJavaScript(parent);
    }else if(extName == "json"){
        return new QsciLexerJSON(parent);
    }else if(extName == "lua"){
        return new QsciLexerLua(parent);
    }else if(fileName == "makefile"){
        qDebug() << "makefile";
        return new QsciLexerMakefile(parent);
    }else if(extName == "makedown" ||extName == "md"  ){
        return new QsciLexerMarkdown(parent);
    }else if(extName == "matlab"){
        return new QsciLexerMatlab(parent);
    }else if(extName == "octave"){
        return new QsciLexerOctave(parent);
    }else if(extName == "pascal"){
        return new QsciLexerPascal(parent);
    }else if(extName == "pl"){
        return new QsciLexerPerl(parent);
    }else if(extName == "po"){
        return new QsciLexerPO(parent);
    }else if(extName == "ps"){
        return new QsciLexerPostScript(parent);
    }else if(extName == "pov"){
        return new QsciLexerPOV(parent);
    }else if(extName == "properties" || fileName == "ini"){
        return new QsciLexerProperties(parent);
    }else if(extName == "py"){
        return new QsciLexerPython(parent);
    }else if(extName == "rb"){
        return new QsciLexerRuby(parent);
    }else if(extName == "sp"){
        return new QsciLexerSpice(parent);
    }else if(extName == "sql"){
        return new QsciLexerSQL(parent);
    }else if(extName == "tcl"){
        return new QsciLexerTCL(parent);
    }else if(extName == "tex"){
        return new QsciLexerTeX(parent);
    }else if(extName == "vlg"){
        return new QsciLexerVerilog(parent);
    }else if(extName == "vhdl"){
        return new QsciLexerVHDL(parent);
    }else if(extName == "xml"){
        return new QsciLexerXML(parent);
    }else if(extName == "yml" || extName == "yaml"){
        return new QsciLexerYAML(parent);
    }

    else if(extName == "jsp"){
        return new QsciLexerHTML(parent);
    }else if(extName == "php"){
        return new QsciLexerHTML(parent);
    }else if(extName == "asp" || extName == "aspx"){
        return new QsciLexerHTML(parent);
    }

    return NULL;
}


CodeEditor::CodeEditor(const QString& filePath,const QString& charset,QWidget *parent):QsciScintilla(parent),
    filePath(filePath),charset(charset),dirty(false),textChangeFlag(-1),selectChangeFlag(0)
{
    QFileInfo fileInfo(filePath);
    //设置语法
    QsciLexer* lexer = getLexer(fileInfo.fileName().toLower(),fileInfo.suffix().toLower(),this);
    if(lexer!=NULL){
        lexer->setFont(QFont("Courier New",10));
        this->setLexer(lexer);//给QsciScintilla设置词法分析器
    }
    this->setTabWidth(4);
    this->SendScintilla(CodeEditor::SCI_SETCODEPAGE,CodeEditor::SC_CP_UTF8);
    this->setCaretLineVisible(true);
    this->setMarginType(0, CodeEditor::NumberMargin);
    this->setMarginLineNumbers(0, true);

    this->indicatorDefine(QsciScintilla::FullBoxIndicator,CodeEditor::Indicator::TemplateSymbol);
    this->setIndicatorForegroundColor(QColor(0,255,0,100),CodeEditor::Indicator::TemplateSymbol);
    this->setIndicatorOutlineColor(QColor(0,255,0),CodeEditor::Indicator::TemplateSymbol);
    this->setIndicatorDrawUnder(true,CodeEditor::Indicator::TemplateSymbol);

    this->indicatorDefine(QsciScintilla::FullBoxIndicator,CodeEditor::Indicator::SelectedHighlight);
    this->setIndicatorForegroundColor(QColor(0,0,255,50),CodeEditor::Indicator::SelectedHighlight);

    this->indicatorDefine(QsciScintilla::FullBoxIndicator,CodeEditor::Indicator::FinderHighlight);
    this->setIndicatorForegroundColor(QColor(0,255,255,70),CodeEditor::Indicator::FinderHighlight);


    QString content = utils::readFileToQString(filePath,charset);
    this->setText(content);

    this->setMarginWidth(0,QString::number(this->lines()).size()*8+4);

    connect(this,&CodeEditor::modificationChanged,this,[=](bool changed){
        if(changed){
            this->dirty = true;
        }else{
            this->dirty = false;
        }
    });
    connect(this,&CodeEditor::textChanged,this,[=](){
        textChangeFlag = QDateTime::currentDateTime().toMSecsSinceEpoch();
//        qDebug()<<"textChangeFlag:" <<textChangeFlag;
        QTimer::singleShot(500, this,[=]{
            if(textChangeFlag!=-1 && (QDateTime::currentDateTime().toMSecsSinceEpoch() - textChangeFlag >=500)){
                textChangeFlag=-1;
                this->refreshTemplateHighlight();
                this->setMarginWidth(0,QString::number(this->lines()).size()*8+4);
            }
        });
    });

    connect(this,&CodeEditor::selectionChanged,this,[=]{
        if(selectChangeFlag==0){
            handleSelectionChanged();
        }
    });
    QTimer::singleShot(100, this,[=]{
        this->refreshTemplateHighlight();
    });
}

CodeEditor::~CodeEditor()
{

}


void CodeEditor::keyPressEvent(QKeyEvent  *event)
{
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_S)){
        editorSave();
        QsciScintilla::keyPressEvent(event);
    }else if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_D)){

        int beginY,beginX,endY,endX;
        this->getSelection(&beginY,&beginX,&endY,&endX); //此次勾选未必对
        if(beginY == -1){
            this->getCursorPosition(&beginY,&beginX);
            endY = beginY;
            endX = beginX;
        }
        qDebug() << "beginY:" << beginY <<"endY:" <<endY;
        this->setSelection(beginY,0,endY+1,0);
        this->replaceSelectedText("");
//        QsciScintilla::keyPressEvent(event);
    }else{
        QsciScintilla::keyPressEvent(event);
    }
}

void CodeEditor::codeEditorInsertSymbol(int codeEditorInsertSymbol)
{
    int beginY,beginX,endY,endX;
    this->getSelection(&beginY,&beginX,&endY,&endX);
//    qDebug()<<"cursor posit:" <<"beginY:" << beginY << "beginY:"<<beginY <<"endY:"<< endY <<"endX:"<< endX;
    if(beginY == -1){
        this->getCursorPosition(&beginY,&beginX);
        endY = beginY;
        endX = beginX;
    }
    if(codeEditorInsertSymbol==TemplateSymbol::VAR){
        if(beginY == endY && endX != beginX){  //行内勾选
            QString word = this->selectedText();
            QString s = "[[[ "+word+" ]]]";
            this->replaceSelectedText(s);
            this->setSelection(beginY,beginX+4,endY,endX+4);
        }else{
            QString s = "[[[ 这里输入变量 ]]]";
            this->insert(s);
//            this->setSelection(endY,endX+4,endY,endX+(s.size()-4));
//            this->setIndicatorOutlineColor(QColor(255,0,0),4);
//            this->fillIndicatorRange(beginY,beginX,endY,endX+s.size(),CodeEditor::Indicator::TemplateSymbol );
//            this->setIndicatorHoverStyle( QsciScintilla::FullBoxIndicator,2);
        }
    }else if(codeEditorInsertSymbol==TemplateSymbol::IF){
        QString s = "[[% if 这里输入条件 %]]";
        QString s2 = "[[% endif %]]";
        if(beginY == endY){
            this->insertAt(s,beginY,beginX);
            this->insertAt(s2,endY,endX+s.size());
            this->setCursorPosition(endY,endX + s.size());
        }else{
            this->insertAt(s,beginY,beginX);
            this->insertAt("\r\n",beginY,beginX+s.size());
            this->insertAt(s2,endY+1,endX);
            this->insertAt("\r\n",endY+1,endX+s2.size());
            this->setCursorPosition(endY+1,endX);
        }
    }else if(codeEditorInsertSymbol==TemplateSymbol::ELSE){
        QString s = "[[% else %]]";
        this->insert(s);
        this->setCursorPosition(endY,endX + s.size());
    }else if(codeEditorInsertSymbol==TemplateSymbol::ELSEIF){
        QString s = "[[% elif 这里输入条件 %]]";
        this->insert(s);
        this->setCursorPosition(endY,endX + s.size());
    }else if(codeEditorInsertSymbol==TemplateSymbol::FOR){
        QString s = "[[% for 变量 in 集合 %]]";
        QString s2 = "[[% endfor %]]";
        if(beginY == endY){
            this->insertAt(s,beginY,beginX);
            this->insertAt(s2,endY,endX+s.size());
            this->setCursorPosition(endY,endX + s.size());
        }else{
            this->insertAt(s,beginY,beginX);
            this->insertAt("\r\n",beginY,beginX+ s.size());
            this->insertAt(s2,endY+1,endX);
            this->insertAt("\r\n",endY+1,endX+ s2.size());
            this->setCursorPosition(endY+1,endX);
        }
    }
}

void CodeEditor::refreshTemplateHighlight()
{
    QString pattern("\\[\\[[\\[%][\\s]+.+[\\s]+[\\]%]\\]\\]");
    QRegExp rx(pattern);
    rx.setMinimal(true);
    qDebug() << "匹配 :" << this->lines();
    int lineLen = this->lines();
    this->clearIndicatorRange(0,0,lineLen,0,CodeEditor::Indicator::TemplateSymbol);

    for(int i=0;i<this->lines();i++){
        QString lineStr = this->text(i);
        int pos = 0;
        while ((pos = rx.indexIn(lineStr , pos)) != -1)
        {
            int start = pos;
            int end = pos+rx.matchedLength();
            pos = end;
            this->fillIndicatorRange(i,start,i,end,CodeEditor::Indicator::TemplateSymbol );
        }
    };


//    bool res = this->findFirst("[[",false,false,false,true);
    //    qDebug()<<"findFirest:"<<res;
}

void CodeEditor::handleSelectionChanged()
{
//    int lineLen = this->lines();
    QString selectText = this->selectedText();
    if(1< selectText.size() && selectText.indexOf("\n")==-1){
        this->clearIndicatorRange(0,0,this->lines(),0,CodeEditor::Indicator::FinderHighlight);
        this->clearIndicatorRange(0,0,this->lines(),0,CodeEditor::Indicator::SelectedHighlight);
//        this->clearIndicatorRange(0,0,lineLen,0,CodeEditor::Indicator::SelectedHighlight);
        for(int i=0;i<this->lines();i++){
            QString lineStr = this->text(i);
            int pos = 0;
            while ((pos = lineStr.indexOf(selectText , pos)) != -1)
            {
                int start = pos;
                int end = pos+selectText.size();
                pos = end;
                this->fillIndicatorRange(i,start,i,end,CodeEditor::Indicator::SelectedHighlight );
            }
        };

    }
}

void CodeEditor::finderHighlight(QString word, bool reg,bool caseSensitive){
    if(!word.isEmpty()){
        this->clearIndicatorRange(0,0,this->lines(),0,CodeEditor::Indicator::SelectedHighlight);
        this->clearIndicatorRange(0,0,this->lines(),0,CodeEditor::Indicator::FinderHighlight);
        QRegularExpression::PatternOption patternOption = caseSensitive? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption ;
        Qt::CaseSensitivity caseSensitivity = caseSensitive? Qt::CaseSensitive : Qt::CaseInsensitive;
        QRegularExpression regularExpression(word,patternOption);
        int lineLen = this->lines();
        if(1< word.size() && word.indexOf("\n")==-1){
            for(int i=0;i< lineLen;i++){
                QString lineStr = this->text(i);
                int pos = 0;
                while (true)
                {
                    int matchLen=0;
                    if(reg){
                        QRegularExpressionMatch match = regularExpression.match(lineStr,pos);
                        if(match.hasMatch()){
                            pos = match.capturedStart();
                            matchLen = match.capturedLength();
                        }else{
                            pos = -1;
                        }
                    }else{
                        pos = lineStr.indexOf(word , pos,caseSensitivity);
                        matchLen = word.size();
                    }
                    if(pos == -1){
                        break;
                    }
                    int start = pos;
                    int end = pos+matchLen;
                    pos = end;
                    this->fillIndicatorRange(i,start,i,end,CodeEditor::Indicator::FinderHighlight );
                }
            };
        }
    }
}

void CodeEditor::finderReplace(int replaceType, QString word,QString toWord, bool reg, bool direction,bool caseSensitive ){
    if(!word.isEmpty()){
        selectChangeFlag++;
        QRegularExpression::PatternOption patternOption = caseSensitive? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption ;
        QRegularExpression regularExpression(word,patternOption);

        int beginY,beginX,endY,endX;
        this->getSelection(&beginY,&beginX,&endY,&endX); //此次勾选未必对
        if(beginY != -1){ //有勾选，归位
            if(direction){
                this->setSelection(beginY,beginX,beginY,beginX);
            }else{
                this->setSelection(endY,endX,endY,endX);
            }
        }
        finderFind(word,reg,direction,caseSensitive); //归位好搜索一次
        this->getSelection(&beginY,&beginX,&endY,&endX); //再检查
        if(beginY != -1){ //有勾选
            QString _toWord;
            if(reg){
                _toWord = this->selectedText().replace(regularExpression,toWord);
            }else{
                _toWord = toWord;
            }
            this->replaceSelectedText(_toWord);
            this->setSelection(beginY,beginX,endY,beginX+_toWord.size());

            if(replaceType==1){

            }else if(replaceType==2){
                finderFind(word,reg,direction,caseSensitive);
            }
        }
        selectChangeFlag--;
    }
}


int CodeEditor::finderReplaceAll(QString word,QString toWord, bool reg,bool caseSensitive )
{
    if(!word.isEmpty()){
        selectChangeFlag++;
        QRegularExpression::PatternOption patternOption = caseSensitive? QRegularExpression::CaseInsensitiveOption : QRegularExpression::NoPatternOption;
        Qt::CaseSensitivity caseSensitivity = caseSensitive? Qt::CaseSensitive : Qt::CaseInsensitive;
        QRegularExpression regularExpression(word,patternOption);

        int num=0;
        for(int i=0;i<this->lines();i++){
            QString lineStr = this->text(i);

            int pos = 0;
            while (true)
            {
                int matchLen=0;
                if(reg){
                    QRegularExpressionMatch match = regularExpression.match(lineStr,pos);
                    if(match.hasMatch()){
                        pos = match.capturedStart();
                        matchLen = match.capturedLength();
                    }else{
                        pos = -1;
                    }
                }else{
                    pos = lineStr.indexOf(word , pos,caseSensitivity);
                    matchLen = word.size();
                }
                if(pos == -1){
                    break;
                }
                int start = pos;
                pos = start+matchLen;
                num++;
            }
        };

        QString text = this->text();
        QString resText;
        if(reg){
            resText = text.replace(regularExpression,toWord);
        }else{
            resText = text.replace(word,toWord,caseSensitivity);
        }
        this->selectAll();
        this->replaceSelectedText(resText);
        selectChangeFlag--;
        return num;
    }
    return 0;
}





void CodeEditor::finderFind(QString word, bool reg, bool direction,bool caseSensitive )
{
    if(!word.isEmpty()){
        selectChangeFlag++;
        QRegularExpression::PatternOption patternOption = caseSensitive? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption ;
        Qt::CaseSensitivity caseSensitivity = caseSensitive? Qt::CaseSensitive : Qt::CaseInsensitive;
        QRegularExpression regularExpression(word,patternOption);

        for(int i=0;i<2;i++){ //两次搜索机会，第一指定开始位置，第二开始或者结束搜索
            int lineRow = 0;
            int indexRow = 0;
            int beginY,beginX,endY,endX;
            this->getSelection(&beginY,&beginX,&endY,&endX);
            if(beginY != -1){ //有勾选
                if(direction){
                    lineRow = endY;
                    indexRow = endX;
                }else{
                    lineRow = beginY;
                    indexRow = beginX;
                }
            }else{
                this->getCursorPosition(&beginY,&beginX);
                if(beginY !=-1 ){ //有光标
                    lineRow = beginY;
                    indexRow = beginX;
                }else{
                    if(direction){
                        lineRow = 0;
                        indexRow = 0;
                    }else{
                        lineRow = this->lines()-1;
                        indexRow = this->lineLength(lineRow);
                    }
                }
            }
            qDebug()<<"cursor posit:" <<"beginY:" << beginY << "beginX:"<<beginX <<"endY:"<< endY <<"endX:"<< endX;
            qDebug()<<"indexRow posit:" <<"lineRow:" << lineRow << "indexRow:"<<indexRow  ;
            bool hasFind = false;
            if(direction){
                for(int i=lineRow;i<this->lines();i++){
                    QString lineStr = this->text(i);
                    int pos = -1;
                    int matchLen=0;
                    if(reg){
                        QRegularExpressionMatch match = regularExpression.match(lineStr,indexRow);
                        if(match.hasMatch()){
                            pos = match.capturedStart();
                            matchLen = match.capturedLength();
                        }else{
                            pos = -1;
                        }
                    }else{
                        pos = lineStr.indexOf(word , indexRow,caseSensitivity);
                        matchLen = word.size();
                    }
                    indexRow=0;
                    if(pos  != -1){
                        int start = pos;
                        this->setSelection(i,start,i,start+matchLen);
                        hasFind=true;
                        break;
                    }
                };
            }else{
                for(int i=lineRow;i>=0;i--){
                    QString lineStr = this->text(i);
                    int pos = -1;
                    int matchLen=0;
                    qDebug()<< "pos:" << pos;
                    if(reg){
                        QRegularExpressionMatch match = regularExpression.match(lineStr.mid(0,indexRow));
                        if(match.hasMatch()){
                            pos = match.capturedStart();
                            matchLen = match.capturedLength();
                        }else{
                            pos = -1;
                        }
                    }else{
                        pos =  lineStr.mid(0,indexRow).lastIndexOf(word,-1,caseSensitivity);
                        matchLen = word.size();
                    }
                    indexRow=-1;
                    if(pos  != -1){
                        int start = pos;
                        this->setSelection(i,start,i,start+matchLen);
                        hasFind=true;
                        break;
                    }
                };
            }
            if(!hasFind){
                if(direction){
                    lineRow = 0;
                    indexRow = 0;
                }else{
                    lineRow = this->lines()-1;
                    indexRow = this->lineLength(lineRow);
                }
                this->setCursorPosition(lineRow,indexRow);
            }else{
                break;
            }
        }
        selectChangeFlag--;
    }
}

QString CodeEditor::editorFilePath()
{
    return filePath;
}


bool CodeEditor::editorClose()
{
    qDebug() <<"editorClose:" << this->dirty;
    if(dirty){
        QString fileName =  QFileInfo(filePath).fileName();
        int res = QMessageBox::warning(this,"警告","关闭前，是否保存 "+fileName+" 文件？",QMessageBox::Yes |QMessageBox::No| QMessageBox::Cancel, QMessageBox::Cancel);
        if(res == QMessageBox::Yes){
            editorSave();
        }else if(res == QMessageBox::No){

        }else{
            return false;
        }
    }
    return true;
}

bool CodeEditor::editorSave()
{
    QFileInfo fileInfo(filePath);
    if(!fileInfo.exists()){
        int res = QMessageBox::warning(this,"警告","文件不存在，是否保存 "+fileInfo.fileName()+" 文件？",QMessageBox::Yes |QMessageBox::No, QMessageBox::No);
        if(res == QMessageBox::Yes){

        }else{
            return false;
        }
    }
    this->SendScintilla(CodeEditor::SCI_SETSAVEPOINT);
    QString content = this->text();
    utils::writeQStringToFile(content,filePath,charset);
    return true;

}
