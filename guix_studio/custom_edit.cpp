#include "studiox_includes.h"
#include "custom_edit.h"

///////////////////////////////////////////////////////////////////////////////
custom_edit::custom_edit()
{
    m_bNegativeValueAllowed = FALSE;
    m_bNumberCheck = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
void custom_edit::ErrorMessage()
{
    ErrorMsg("Unacceptable Character. You can only type a number here.", this);
}

///////////////////////////////////////////////////////////////////////////////
BOOL custom_edit::OnPaste()
{
    if (!m_bNumberCheck)
    {
        return FALSE;
    }

    if (OpenClipboard())
    {
        BOOL invalid_input = FALSE;
        HANDLE hMem = GetClipboardData(CF_UNICODETEXT);

        if (!hMem)
        {
            CloseClipboard();
            ErrorMsg("Unknown clipboard data format.", this);
            return NULL;
        }

        LPTSTR lptstr = (LPTSTR)GlobalLock(hMem);

        if (lptstr)
        {
            CString string;
            INT start, end;
            GetWindowText(string);

            // Pickup selection range.
            SendMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);

            // Prapare string for checking.
            if (start != end)
            {
                string.Delete(start, end - start);
            }
            string.Insert(start, lptstr);

            // Check invalid string.
            wchar_t character;
            for (int index = 0; index < string.GetLength(); index++)
            {
                character = string.GetAt(index);

                if (!(m_bNegativeValueAllowed) || (index != 0) || (character != '-'))
                {
                    if (character < '0' || character > '9')
                    {
                        // Only numeric value is allowed.
                        invalid_input = TRUE;
                        break;
                    }
                }
            }
        }

        GlobalUnlock(hMem);

        CloseClipboard();

        if (invalid_input)
        {
            ErrorMessage();
            return TRUE;
        }
    }
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL custom_edit::OnChar(UINT key)
{
    if (!m_bNumberCheck)
    {
        return FALSE;
    }

    if (!isprint(key))
    {
        return FALSE;
    }

    BOOL input_invalid = FALSE;

    if (m_bNegativeValueAllowed && (key == '-'))
    {
        CString string;
        INT start, end;
        GetWindowTextW(string);
        if (!string.IsEmpty())
        {
            // Pickup selection range.
            SendMessage(EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
            if (start == 0)
            {
                if ((string.GetAt(0) == '-') && (start == end))
                {
                    // Insert negative sign is not allowed,
                    // as the first character is already a negative sign.
                    input_invalid = TRUE;
                }
            }
            else
            {
                // Insert negative sign in the middle of a numeric string is not allowed.
                input_invalid = TRUE;
            }
        }
    }
    else
    {
        if (key < '0' || key > '9')
        {
            // Only numeric value is allowed.
            input_invalid = TRUE;
        }
    }

    if (input_invalid)
    {
        // Prompt error message.
        ErrorMessage();
        return TRUE;
    }

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
LRESULT custom_edit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PASTE:
        if (OnPaste())
        {
            return TRUE;
        }
        break;

    case WM_CHAR:
        if (OnChar(wParam))
        {
            return TRUE;
        }
        break;
    }
    return CEdit::WindowProc(message, wParam, lParam);
}
