#ifndef SWITCHFUNCTIONS_H
#define SWITCHFUNCTIONS_H


//Function to check and toggle switches as needed
void MainWindow::checkSwitches(QString cN, QString nN)
{
    if (cN == "36")
    {
        if(nN == "35")
            setSwitch("11", "Open");
        else if(nN == "68")
           setSwitch("11", "Bypass");
        else
           return;
    }
    else if (cN == "39")
    {
        if(nN == "38")
            setSwitch("21", "Open");
        else if(nN == "37")
           setSwitch("21", "Bypass");
        else
           return;
    }
    else if (cN == "65")
    {
        if(nN == "66")
            setSwitch("43", "Open");
        else if(nN == "67")
           setSwitch("43", "Bypass");
        else
           return;
    }
    else if (cN == "31")
    {
        if(nN == "70")
            setSwitch("42", "Open");
        else if(nN == "30")
           setSwitch("42", "Bypass");
        else
           return;
    }
    else if (cN == "32")
    {
        if(nN == "33")
            setSwitch("13", "Open");
        else if(nN == "34")
          {
           setSwitch("13", "Bypass");
           setSwitch("12", "Bypass");
          }
        else
           return;
    }
    else if (cN == "34")
    {
        if(nN == "50")
            setSwitch("12", "Open");
        else if(nN == "32")
            {
             setSwitch("13", "Bypass");
             setSwitch("12", "Bypass");
            }
        else
           return;
    }
    else if (cN == "0")
    {
        if(nN == "1")
            setSwitch("22", "Open");
        else if(nN == "2")
           {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
           }
        else
           return;
    }
    else if (cN == "2")
    {
        if(nN == "71")
            setSwitch("23", "Open");
        else if(nN == "0")
            {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
            }
        else
           return;
    }
    else if (cN == "29")
    {
        if(nN == "21")
            setSwitch("24", "Open");
        else if(nN == "28")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (cN == "28")
    {
        if(nN == "26")
            setSwitch("34", "Open");
        else if(nN == "27")
           setSwitch("34", "Bypass");
        else if(nN == "29")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (cN == "26")
    {
        if(nN == "51")
            setSwitch("33", "Open");
        else if(nN == "25")
           setSwitch("33", "Bypass");
        else if(nN == "28")
            setSwitch("34", "Open");
        else
           return;
    }
    else if (cN == "51")
    {
        if(nN == "52")
            setSwitch("32", "Open");
        else if(nN == "55")
           setSwitch("32", "Bypass");
        else if(nN == "26")
            setSwitch("33", "Open");
        else
           return;
    }
    else if (cN == "52")
    {
        if(nN == "53")
            setSwitch("31", "Open");
        else if(nN == "54")
           setSwitch("31", "Bypass");
        else if(nN == "51")
            setSwitch("32", "Open");
        else
           return;
    }
    else if (cN == "49")
    {
        if(nN == "47")
            setSwitch("41", "Open");
        else if(nN == "48")
           setSwitch("41", "Bypass");
        else
           return;
    }
    else if (cN == "56")
    {
        if(nN == "57")
            setSwitch("14", "Open");
        else if(nN == "58")
           setSwitch("14", "Bypass");
        else
           return;
    }
    else if (cN == "42")
    {
        if(nN == "43")
            setSwitch("63", "Open");
        else if(nN == "44")
           setSwitch("63", "Bypass");
        else if(nN == "41")
            setSwitch("62", "Open");
        else
           return;
    }
    else if (cN == "41")
    {
        if(nN == "42")
            setSwitch("62", "Open");
        else if(nN == "46")
           setSwitch("62", "Bypass");
        else if(nN == "40")
            setSwitch("61", "Open");
        else
           return;
    }
    else if (cN == "40")
    {
        if(nN == "41")
            setSwitch("61", "Open");
        else if(nN == "45")
           setSwitch("61", "Bypass");
        else
           return;
    }
    else if (cN == "61")
    {
        if(nN == "59")
            setSwitch("84", "Open");
        else if(nN == "60")
           setSwitch("84", "Bypass");
        else
           return;
    }
    else if (cN == "22")
    {
        if(nN == "24")
            setSwitch("64", "Open");
        else if(nN == "23")
           setSwitch("64", "Bypass");
        else if(nN == "20")
            setSwitch("75","Bypass");
        else
           return;
    }
    else if (cN == "20")
    {
        if(nN == "21")
            setSwitch("74", "Open");
        else if(nN == "22")
           setSwitch("74", "Bypass");
        else
           return;
    }
    else if (cN == "4")
    {
        if(nN == "6")
            setSwitch("73", "Open");
        else if(nN == "5")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (cN == "5")
    {
        if(nN == "3")
            setSwitch("72", "Open");
        else if(nN == "4")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (cN == "7")
    {
        if(nN == "9")
            setSwitch("71", "Open");
        else if(nN == "8")
           setSwitch("71", "Bypass");
        else
           return;
    }
    else if (cN == "62")
    {
        if(nN == "63")
            setSwitch("52", "Open");
        else if(nN == "64")
           setSwitch("52", "Bypass");
        else
           return;
    }
    else if (cN == "17")
    {
        if(nN == "19")
            setSwitch("51", "Open");
        else if(nN == "18")
           setSwitch("51", "Bypass");
        else
           return;
    }
    else if (cN == "16")
    {
        if(nN == "12")
            setSwitch("83", "Open");
        else if(nN == "14")
        {
           setSwitch("83", "Bypass");
           setSwitch("82", "Bypass");
        }
        else
           return;
    }
    else if (cN == "14")
    {
        if(nN == "15")
            setSwitch("82", "Open");
        else if(nN == "16")
        {
           setSwitch("82", "Bypass");
           setSwitch("83", "Bypass");
        }
        else
           return;
    }
    else if (cN == "10")
    {
        if(nN == "12")
            setSwitch("81", "Open");
        else if(nN == "11")
           setSwitch("81", "Bypass");
        else
           return;
    }
    else if (cN == "35" && nN == "36")
        setSwitch("11", "Open");
    else if (cN == "38" && nN == "39")
        setSwitch("21", "Open");
    else if (cN == "66" && nN == "65")
        setSwitch("43", "Open");
    else if (cN == "70" && nN == "31")
        setSwitch("42", "Open");
    else if (cN == "33" && nN == "32")
        setSwitch("13", "Open");
    else if (cN == "50" && nN == "34")
        setSwitch("12", "Open");
    else if (cN == "1" && nN == "0")
        setSwitch("22", "Open");
    else if (cN == "71" && nN == "2")
        setSwitch("23", "Open");
    else if (cN == "53" && nN == "52")
        setSwitch("31", "Open");
    else if (cN == "47" && nN == "49")
        setSwitch("41", "Open");
    else if (cN == "57" && nN == "56")
        setSwitch("14", "Open");
    else if (cN == "43" && nN == "42")
        setSwitch("63", "Open");
    else if (cN == "59" && nN == "61")
        setSwitch("84", "Open");
    else if (cN == "24" && nN == "22")
        setSwitch("64", "Open");
    else if (cN == "6" && nN == "4")
        setSwitch("73", "Open");
    else if (cN == "3" && nN == "5")
        setSwitch("72", "Open");
    else if (cN == "9" && nN == "7")
        setSwitch("71", "Open");
    else if (cN == "63" && nN == "62")
        setSwitch("52", "Open");
    else if (cN == "19" && nN == "17")
        setSwitch("51", "Open");
    else if (cN == "15" && nN == "14")
        setSwitch("82", "Open");
    else if(cN == "21")
    {
        if (nN == "20")
            setSwitch("74", "Open");
        else if (nN == "29")
            setSwitch("24", "Open");
        else
            return;
    }
    else if(cN == "12")
    {
        if (nN == "16")
            setSwitch("83", "Open");
        else if (nN == "10")
            setSwitch("81", "Open");
        else
            return;
    }
    else if (cN == "68" && nN == "36")
        setSwitch("11", "Bypass");
    else if (cN == "37" && nN == "39")
        setSwitch("21", "Bypass");
    else if (cN == "67" && nN == "65")
        setSwitch("43", "Bypass");
    else if (cN == "30" && nN == "31")
        setSwitch("42", "Bypass");
    else if (cN == "27" && nN == "28")
        setSwitch("34", "Bypass");
    else if (cN == "25" && nN == "26")
        setSwitch("33", "Bypass");
    else if (cN == "55" && nN == "51")
        setSwitch("32", "Bypass");
    else if (cN == "54" && nN == "52")
        setSwitch("31", "Bypass");
    else if (cN == "48" && nN == "49")
        setSwitch("41", "Bypass");
    else if (cN == "58" && nN == "56")
        setSwitch("14", "Bypass");
    else if (cN == "44" && nN == "42")
        setSwitch("63", "Bypass");
    else if (cN == "46" && nN == "41")
        setSwitch("62", "Bypass");
    else if (cN == "45" && nN == "40")
        setSwitch("61", "Bypass");
    else if (cN == "60" && nN == "61")
        setSwitch("84", "Bypass");
    else if (cN == "23" && nN == "22")
        setSwitch("64", "Bypass");
    else if (cN == "8" && nN == "7")
        setSwitch("71", "Bypass");
    else if (cN == "64" && nN == "62")
        setSwitch("52", "Bypass");
    else if (cN == "18" && nN == "17")
        setSwitch("51", "Bypass");
    else if (cN == "11" && nN == "10")
        setSwitch("81", "Bypass");

    return;
}

void MainWindow::setSwitch(QString sN, QString sM)
{
    QSqlQuery q(ldb);
    q.prepare("UPDATE switches SET position=? WHERE switchID=?");
    q.bindValue(0, sM);
    q.bindValue(1, sN);

    if(!q.exec())
    {
    cout << "Failed to update database";
    }
}



#endif // SWITCHFUNCTIONS_H
