#ifndef DIJKSTRASWITCHCHECK_H
#define DIJKSTRASWITCHCHECK_H
#include <iostream>
#include <Dijkstra.h>


//function to reject using the same switch two times in a row

long MainWindow::dijcheckSwitches(long dcN, long dnN)
{
    int switchUsed;
    if (dcN == 36)
    {
        if(dnN == 35)
            switchUsed = 11;
        else if(dnN == 68)
           switchUsed = 11;

        else
           return switchUsed;
    }
    else if (dcN == 39 || dcN == 74)
    {
        if(dnN == 38)
            switchUsed = 21;
        else if(dnN == 37)
           switchUsed = 21;
        else
           return switchUsed;
    }
    /*else if (dcN == "65")
    {
        if(dnN == "66")
            setSwitch("43", "Pass");
        else if(dnN == "67")
           setSwitch("43", "Bypass");
        else
           return;
    }
    else if (dcN == "31")
    {
        if(dnN == "70")
            setSwitch("42", "Pass");
        else if(dnN == "30")
           setSwitch("42", "Bypass");
        else if(dnN == "32")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "32")
    {
        if(dnN == "33")
            setSwitch("13", "Pass");
        else if(dnN == "34")
          {
           setSwitch("13", "Bypass");
           setSwitch("12", "Bypass");
          }
        else if(dnN == "31")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "34")
    {
        if(dnN == "50")
            setSwitch("12", "Pass");
        else if(dnN == "32")
            {
             setSwitch("13", "Bypass");
             setSwitch("12", "Bypass");
            }
        else
           return;
    }
    else if (dcN == "0" || dcN == "75")
    {
        if(dnN == "1")
            setSwitch("22", "Pass");
        else if(dnN == "2")
           {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
           }
        else
           return;
    }
    else if (dcN == "2")
    {
        if(dnN == "71")
            setSwitch("23", "Pass");
        else if(dnN == "0" || dnN == "75")
            {
            setSwitch("22", "Bypass");
            setSwitch("23", "Bypass");
            }
        else
           return;
    }
    else if (dcN == "29")
    {
        if(dnN == "21")
            setSwitch("24", "Pass");
        else if(dnN == "28")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (dcN == "28")
    {
        if(dnN == "26")
            setSwitch("34", "Pass");
        else if(dnN == "27")
           setSwitch("34", "Bypass");
        else if(dnN == "29")
           setSwitch("24", "Bypass");
        else
           return;
    }
    else if (dcN == "26")
    {
        if(dnN == "51")
            setSwitch("33", "Pass");
        else if(dnN == "25")
           setSwitch("33", "Bypass");
        else if(dnN == "28")
            setSwitch("34", "Pass");
        else
           return;
    }
    else if (dcN == "51")
    {
        if(dnN == "52")
            setSwitch("32", "Pass");
        else if(dnN == "55"){
           setSwitch("32", "Bypass");
           cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else if(dnN == "26")
            setSwitch("33", "Pass");
        else
           return;
    }
    else if (dcN == "52")
    {
        if(dnN == "53"){
            setSwitch("31", "Pass");
        cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else if(dnN == "54"){
           setSwitch("31", "Bypass");
        cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else if(dnN == "51")
            setSwitch("32", "Pass");
        else
           return;
    }
    else if (dcN == "49")
    {
        if(dnN == "47")
            setSwitch("41", "Pass");
        else if(dnN == "48")
           setSwitch("41", "Bypass");
        else if(dnN == "50")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "56")
    {
        if(dnN == "57")
            setSwitch("14", "Pass");
        else if(dnN == "58")
           setSwitch("14", "Bypass");
        else
           return;
    }
    else if (dcN == "42")
    {
        if(dnN == "43"){
            setSwitch("63", "Pass");
        cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else if(dnN == "44"){
           setSwitch("63", "Bypass");
           cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else if(dnN == "41")
            setSwitch("62", "Pass");
        else
           return;
    }
    else if (dcN == "41")
    {
        if(dnN == "42")
            setSwitch("62", "Pass");
        else if(dnN == "46")
           setSwitch("62", "Bypass");
        else if(dnN == "40")
            setSwitch("61", "Bypass");
        else
           return;
    }
    else if (dcN == "40")
    {
        if(dnN == "41")
            setSwitch("61", "Pass");
        else if(dnN == "45")
           setSwitch("61", "Bypass");
        else if(dnN == "15")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "61")
    {
        if(dnN == "59")
            setSwitch("84", "Pass");
        else if(dnN == "60")
           setSwitch("84", "Bypass");
        else
           return;
    }
    else if (dcN == "22")
    {
        if(dnN == "24")
            setSwitch("64", "Pass");
        else if(dnN == "23")
           setSwitch("64", "Bypass");
        else if(dnN == "20")
            setSwitch("75","Bypass");
        else
           return;
    }
    else if (dcN == "20")
    {
        if(dnN == "21")
            setSwitch("74", "Pass");
        else if(dnN == "22")
           setSwitch("74", "Bypass");
        else
           return;
    }
    else if (dcN == "4")
    {
        if(dnN == "6")
            setSwitch("73", "Pass");
        else if(dnN == "5" || dnN == "72")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (dcN == "5" || dcN == "72")
    {
        if(dnN == "3")
            setSwitch("72", "Pass");
        else if(dnN == "4")
        {
           setSwitch("73", "Bypass");
           setSwitch("72", "Bypass");
        }
        else
           return;
    }
    else if (dcN == "7" || dcN == "73")
    {
        if(dnN == "9")
            setSwitch("71", "Pass");
        else if(dnN == "8")
           setSwitch("71", "Bypass");
        else
           return;
    }
    else if (dcN == "62")
    {
        if(dnN == "63")
            setSwitch("52", "Pass");
        else if(dnN == "64")
           setSwitch("52", "Bypass");
        else
           return;
    }
    else if (dcN == "17")
    {
        if(dnN == "19")
            setSwitch("51", "Pass");
        else if(dnN == "18")
           setSwitch("51", "Bypass");
        else if(dnN == "16")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "16")
    {
        if(dnN == "12")
            setSwitch("83", "Pass");
        else if(dnN == "14")
        {
           setSwitch("83", "Bypass");
           setSwitch("82", "Bypass");
        }
        else if(dnN == "17")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
        else
           return;
    }
    else if (dcN == "14")
    {
        if(dnN == "15")
            setSwitch("82", "Pass");
        else if(dnN == "16")
        {
           setSwitch("82", "Bypass");
           setSwitch("83", "Bypass");
        }
        else
           return;
    }
    else if (dcN == "10")
    {
        if(dnN == "12")
            setSwitch("81", "Pass");
        else if(dnN == "11"){
           setSwitch("81", "Bypass");
           cout << "ADD THROTTLE DOWN HERE!!! \n";}
        else
           return;
    }
    else if (dcN == "35" && dnN == "36")
        setSwitch("11", "Pass");
    else if (dcN == "38" && (dnN == "39" || dnN == "74"))
        setSwitch("21", "Pass");
    else if (dcN == "66" && dnN == "65")
        setSwitch("43", "Pass");
    else if (dcN == "70" && dnN == "31")
        setSwitch("42", "Pass");
    else if (dcN == "33" && dnN == "32")
        setSwitch("13", "Pass");
    else if (dcN == "50")
    {
        if(dnN == "34")
            setSwitch("12", "Pass");
        else if(dnN == "49")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
    }
    else if (dcN == "1" && (dnN == "0" || dnN == "75"))
        setSwitch("22", "Pass");
    else if (dcN == "71" && dnN == "2")
        setSwitch("23", "Pass");
    else if (dcN == "53" && dnN == "52")
        setSwitch("31", "Pass");
    else if (dcN == "47" && dnN == "49")
        setSwitch("41", "Pass");
    else if (dcN == "57" && dnN == "56")
        setSwitch("14", "Pass");
    else if (dcN == "43" && dnN == "42")
        setSwitch("63", "Pass");
    else if (dcN == "59" && dnN == "61")
        setSwitch("84", "Pass");
    else if (dcN == "24" && dnN == "22")
        setSwitch("64", "Pass");
    else if (dcN == "6" && dnN == "4")
        setSwitch("73", "Pass");
    else if (dcN == "3" && dnN == "5")
        setSwitch("72", "Pass");
    else if (dcN == "9" && (dnN == "7" || dnN == "73"))
        setSwitch("71", "Pass");
    else if (dcN == "63" && dnN == "62")
        setSwitch("52", "Pass");
    else if (dcN == "19" && dnN == "17")
        setSwitch("51", "Pass");
    else if (dcN == "15")
    {
        if(dnN == "14")
            setSwitch("82", "Pass");
        else if (dnN == "40")
            cout << "ADD FUNCTION TO CHECK FOR TRAINS APPROACHING CROSSING!!! \n";
    }
    else if(dcN == "21")
    {
        if (dnN == "20")
            setSwitch("74", "Pass");
        else if (dnN == "29")
            setSwitch("24", "Pass");
        else
            return;
    }
    else if(dcN == "12")
    {
        if (dnN == "16")
            setSwitch("83", "Pass");
        else if (dnN == "10")
            setSwitch("81", "Pass");
        else
            return;
    }
    else if (dcN == "68" && dnN == "36")
        setSwitch("11", "Bypass");
    else if (dcN == "37" && (dnN == "39" || dnN == "74"))
        setSwitch("21", "Bypass");
    else if (dcN == "67" && dnN == "65")
        setSwitch("43", "Bypass");
    else if (dcN == "30" && dnN == "31")
        setSwitch("42", "Bypass");
    else if (dcN == "27" && dnN == "28")
        setSwitch("34", "Bypass");
    else if (dcN == "25" && dnN == "26")
        setSwitch("33", "Bypass");
    else if (dcN == "55" && dnN == "51")
        setSwitch("32", "Bypass");
    else if (dcN == "54" && dnN == "52")
        setSwitch("31", "Bypass");
    else if (dcN == "48" && dnN == "49")
        setSwitch("41", "Bypass");
    else if (dcN == "58" && dnN == "56")
        setSwitch("14", "Bypass");
    else if (dcN == "44" && dnN == "42")
        setSwitch("63", "Bypass");
    else if (dcN == "46" && dnN == "41")
        setSwitch("62", "Bypass");
    else if (dcN == "45" && dnN == "40")
        setSwitch("61", "Bypass");
    else if (dcN == "60" && dnN == "61")
        setSwitch("84", "Bypass");
    else if (dcN == "23" && dnN == "22")
        setSwitch("64", "Bypass");
    else if (dcN == "8" && (dnN == "7" || dnN == "73"))
        setSwitch("71", "Bypass");
    else if (dcN == "64" && dnN == "62")
        setSwitch("52", "Bypass");
    else if (dcN == "18" && dnN == "17")
        setSwitch("51", "Bypass");
    else if (dcN == "11" && dnN == "10")
        setSwitch("81", "Bypass");

    return;
}
   */
}


#endif // DIJKSTRASWITCHCHECK_H
