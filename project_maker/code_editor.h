#ifndef CODE_EDITOR_H
#define CODE_EDITOR_H

#include <Qsci/qsciscintilla.h>
#include "project_maker/ieditor.h"
#include "common/macro_tools.h"

class CodeEditor :public QsciScintilla,public IEditor
{
    Q_OBJECT

public:
    struct TemplateSymbol{
        const static int VAR = 0;
        const static int IF = 1;
        const static int ELSE = 2;
        const static int ELSEIF = 3;
        const static int FOR = 4;
    };
    struct Indicator{
        const static int TemplateSymbol = 9;
        const static int SelectedHighlight = 10;
        const static int FinderHighlight = 11;
    };
    struct CodeEditEnum {
        constexpr static char* type = "type";
        constexpr static char*  dirty = "dirty";
        constexpr static char*  path = "path";
    };


    explicit CodeEditor(const QString& filePath, const QString& charset ="utf-8",QWidget *parent = 0);
    virtual ~CodeEditor();

    virtual bool editorClose();
    virtual bool editorSave();
    virtual QString editorFilePath();

    void keyPressEvent(QKeyEvent  *event);

    void codeEditorInsertSymbol(int codeEditorInsertSymbol);

    void refreshTemplateHighlight();

    void handleSelectionChanged();

    void finderHighlight(QString word, bool reg,bool caseSensitive);
    void finderReplace(int replaceType, QString word,QString toWord, bool reg, bool direction,bool caseSensitive );
    int finderReplaceAll(QString word,QString toWord, bool reg,bool caseSensitive );
    void finderFind(QString word, bool reg, bool direction,bool caseSensitive);

private:
    QString filePath;
    QString charset;
//    GETSET_VALUE(public,bool, dirty)
    bool dirty;
    qint64 textChangeFlag;
    int selectChangeFlag;
public:


};

#endif // CODE_EDITOR_H
