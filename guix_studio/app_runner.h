
/* Logic to run the application in a standalone window */


class app_runner
{
    public:
        app_runner();
        ~app_runner();
        void RunApplication(int display_index, CWnd *parent = NULL);

    private:
        BOOL HaveStartupScreens(const CArray<widget_info *> *screen_list) const;
};
