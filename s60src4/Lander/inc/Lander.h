//Lander.h

class CApplication;
class CDocument;
class CAppUi;
class CAppView;

//
class CApplication : public CAknApplication
{
 public:
        TUid AppDllUid() const;

 protected:
        CApaDocument* CreateDocumentL();
};

//
class CDocument : public CAknDocument
{
 public:
        static CDocument* NewL(CEikApplication& aApp);
        virtual ~CDocument();
        CEikAppUi* CreateAppUiL();

 private:
        void ConstructL();
        CDocument(CEikApplication& aApp);
};

//
class CAppUi : public CAknAppUi
{
 public:
        void ConstructL();
        CAppUi();
        virtual ~CAppUi();
        CAppView* iAppView;

 private:
        TKeyResponse HandleKeyEventL(const TKeyEvent& kevent,TEventCode ecode);
        void HandleCommandL(TInt aCommand);
		void HandleStatusPaneSizeChange();
};
    
//
class CAppView : public CCoeControl
{
 public:
        static CAppView* NewL(const TRect& aRect);
        static CAppView* NewLC(const TRect& aRect);
        virtual ~CAppView();
        void Draw(const TRect& aRect) const;
        virtual void SizeChanged();

 private:
        void ConstructL(const TRect& aRect);
        CAppView();

};
