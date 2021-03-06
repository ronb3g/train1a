#ifndef CHECKPATHS_H
#define CHECKPATHS_H
#include <mainwindow.h>
#include <tableOperations.h>



template <class T> //Simple int-to-string convertor.
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}


int MainWindow::checkPaths(int attempt, int islong) //Checks the paths for collisions.
{
    
    //these variables indicate which engines are active.
    int f1 = 0;
    int f2 = 0;
    int f3 = 0;
    int f4 = 0;
    int f5 = 0;



    //Check to see which paths have been planned so far
    if ( Eng1 != NULL && (ui->setButton1->isChecked())&& pathcomplete1 != 1) //If pathcomplete = 1, that means the route has already started and should not be started over. This mainly comes up with the delay function.
    {

        f1 = 1;
        //comparisonArray = new int[comparisonArray.size()];
    }
    if ( Eng2 != NULL && (ui->setButton2->isChecked()) && pathcomplete2 != 1)
    {
        f2 = 1;
        //comparisonArray2 = new int[comparisonArray2.size()];
    }
    if ( Eng3 != NULL && (ui->setButton3->isChecked())&& pathcomplete3 != 1)
    {
        f3 = 1;
       //comparisonArray3 = new int[comparisonArray3.size()];
    }
    if ( Eng4 != NULL && (ui->setButton4->isChecked()) && pathcomplete4 != 1)
    {
        f4 = 1;
        //comparisonArray4 = new int[comparisonArray4.size()];
    }
    if ( Eng5 != NULL && (ui->setButton5->isChecked())&& pathcomplete5 != 1)
    {
        f5 = 1;
       // comparisonArray5 = new int[comparisonArray5.size()];
   }




//if(pathcomplete1 == 1)
//{
//    Eng1Time(attempt, islong);
//}
//if(pathcomplete2 == 1)
//{
//    Eng2Time(attempt, islong);
//}
//if(pathcomplete3 == 1)
//{
//    Eng3Time(attempt, islong);
//}
//if(pathcomplete4 == 1)
//{
//    Eng4Time(attempt, islong);
//}
//if(pathcomplete5 == 1)
//{
//    Eng5Time(attempt, islong);
//}

    if ( (f1+f2+f3+f4+f5 == 1) || (f1+f2+f3+f4+f5 == 0))
    {
        //No paths or only one path detected. No comparison needed (However, time still needs to be called.)
        if(f1 == 1)
        {
            Eng1Time(attempt, islong);
        }
        else if (f2 == 1)
        {
            Eng2Time(attempt, islong);
        }
        else if (f3 == 1)
        {
            Eng3Time(attempt, islong);
        }
        else if (f4 == 1)
        {
            Eng4Time(attempt, islong);
        }
        else if (f5 == 1)
        {
            Eng5Time(attempt, islong);
        }
        return -1;
    }
    else if (f1+f2+f3+f4+f5 == 2)
    { //2 paths detected. Determining possible collisions
        
        //First, we determine which two trains are active. Then, we determine if they will collide.

        if(f1+f2 == 2)
        {

            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            return (compare2(comparisonArray, comparisonArray2, comparisonArray.size(), comparisonArray2.size()));
        }
        else if (f1+f3 == 2)
        {
            Eng1Time(attempt, islong);
            Eng3Time(attempt, islong);
            return (compare2(comparisonArray, comparisonArray3, comparisonArray.size(), comparisonArray3.size()));
        }
        else if (f1+f4 == 2)
        {
            Eng1Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare2(comparisonArray,comparisonArray4, comparisonArray.size(), comparisonArray4.size()));
        }
        else if(f1+f5 == 2)
        {
            Eng1Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare2(comparisonArray,comparisonArray5,comparisonArray.size(),comparisonArray5.size()));
        }
        else if(f2+f3 == 2)
        {
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            return (compare2(comparisonArray2,comparisonArray3,comparisonArray2.size(),comparisonArray3.size()));
        }
        else if(f2+f4 == 2)
        {
            Eng2Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare2(comparisonArray2,comparisonArray4,comparisonArray2.size(),comparisonArray4.size()));
        }
        else if(f2+f5 == 2)
        {
            Eng2Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare2(comparisonArray2,comparisonArray5,comparisonArray2.size(),comparisonArray5.size()));
        }
        else if(f3+f4 == 2)
        {
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare2(comparisonArray3,comparisonArray4,comparisonArray3.size(),comparisonArray4.size()));
        }
        else if(f3+f5 == 2)
        {
            Eng3Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare2(comparisonArray3,comparisonArray5,comparisonArray3.size(),comparisonArray5.size()));
        }
        else if(f4+f5 == 2)
        {
            Eng4Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare2(comparisonArray4,comparisonArray5,comparisonArray4.size(),comparisonArray5.size()));
        }
    }
    else if (f1+f2+f3+f4+f5 == 3)
      {// 3 paths detected. Checking for collisions.
        
        //Same thing here. If there are 3 paths, we find out which 3, then compare their routes.
        if(f1+f2+f3 == 3)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray2,comparisonArray3,comparisonArray.size(),comparisonArray2.size(),comparisonArray3.size()));
        }
        else if (f1+f2+f4 == 3)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray2,comparisonArray4,comparisonArray.size(),comparisonArray2.size(),comparisonArray4.size()));
        }
        else if (f1+f2+f5 == 3)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray2,comparisonArray5,comparisonArray.size(),comparisonArray2.size(),comparisonArray5.size()));
        }
        else if(f1+f3+f4 == 3)
        {
            Eng1Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray3,comparisonArray4,comparisonArray.size(),comparisonArray3.size(),comparisonArray4.size()));
        }
        else if(f1+f3+f5 == 3)
        {
            Eng1Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray3,comparisonArray5,comparisonArray.size(),comparisonArray3.size(),comparisonArray5.size()));
        }
        else if(f1+f4+f5 == 3)
        {
            Eng1Time(attempt, islong);
            Eng4Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray,comparisonArray4,comparisonArray5,comparisonArray.size(),comparisonArray4.size(),comparisonArray5.size()));
        }
        else if(f2+f3+f4 == 3)
        {
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            return (compare3(comparisonArray2,comparisonArray3,comparisonArray4,comparisonArray2.size(),comparisonArray3.size(),comparisonArray4.size()));
        }
        else if(f2+f3+f5 == 3)
        {
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray2,comparisonArray3,comparisonArray5,comparisonArray2.size(),comparisonArray3.size(),comparisonArray5.size()));
        }
        else if(f2+f4+f5 == 3)
        {
            Eng2Time(attempt, islong);
            Eng4Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray2,comparisonArray4,comparisonArray5,comparisonArray2.size(),comparisonArray4.size(),comparisonArray5.size()));
        }
        else if(f3+f4+f5 == 3)
        {
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            Eng5Time(attempt, islong);
            return (compare3(comparisonArray3,comparisonArray4,comparisonArray5,comparisonArray3.size(),comparisonArray4.size(),comparisonArray5.size()));
        }
    }
    else if(f1+f2+f3+f4+f5 == 4)
    {//4 paths detected. checking now.
        
        //The following code detects which 4 paths are active (if there are exactly 4 paths active), and then compares their routes.
        if (f1+f2+f3+f4 == 4)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            //compare4(comparisonArray,comparisonArray2,comparisonArray3,comparisonArray4,comparisonArray.size(),comparisonArray2.size(),comparisonArray3.size(),comparisonArray4.size());
            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, comparisonArray.size(), comparisonArray2.size(), comparisonArray3.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray4, comparisonArray2.size(), comparisonArray3.size(), comparisonArray4.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray4, comparisonArray.size(), comparisonArray4.size()));

                }
            }


        }
        else if (f2+f3+f4+f5 == 4)
        {
            Eng5Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            //compare4(comparisonArray2,comparisonArray3,comparisonArray4,comparisonArray5,comparisonArray2.size(),comparisonArray3.size(),comparisonArray4.size(),comparisonArray5.size());
            int c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray4, comparisonArray2.size(), comparisonArray3.size(), comparisonArray4.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray3, comparisonArray4, comparisonArray5, comparisonArray3.size(), comparisonArray4.size(), comparisonArray5.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray2, comparisonArray5, comparisonArray2.size(), comparisonArray5.size()));

                }
            }
        }
        else if (f3+f4+f5+f1 == 4)
        {
            Eng1Time(attempt, islong);
            Eng5Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng4Time(attempt, islong);
            //compare4(comparisonArray,comparisonArray3,comparisonArray4,comparisonArray5,comparisonArray.size(),comparisonArray3.size(),comparisonArray4.size(),comparisonArray5.size());
            int c1 = compare3(comparisonArray, comparisonArray4, comparisonArray3, comparisonArray.size(), comparisonArray4.size(), comparisonArray3.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray3, comparisonArray4, comparisonArray5, comparisonArray3.size(), comparisonArray4.size(), comparisonArray5.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, comparisonArray.size(), comparisonArray5.size()));

                }
            }
        }
        else if(f4+f5+f1+f2 == 4)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng5Time(attempt, islong);
            Eng4Time(attempt, islong);
            //compare4(comparisonArray,comparisonArray2,comparisonArray4,comparisonArray5,comparisonArray.size(),comparisonArray2.size(),comparisonArray4.size(),comparisonArray5.size());
            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray4, comparisonArray.size(), comparisonArray2.size(), comparisonArray4.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray4, comparisonArray5, comparisonArray2.size(), comparisonArray4.size(), comparisonArray5.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, comparisonArray.size(), comparisonArray5.size()));

                }
            }

        }
        else if (f1+f2+f3+f5 == 4)
        {
            Eng1Time(attempt, islong);
            Eng2Time(attempt, islong);
            Eng3Time(attempt, islong);
            Eng5Time(attempt, islong);
            //compare4(comparisonArray,comparisonArray2,comparisonArray3,comparisonArray5,comparisonArray.size(),comparisonArray2.size(),comparisonArray3.size(),comparisonArray5.size());

            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, comparisonArray.size(), comparisonArray2.size(), comparisonArray3.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray5, comparisonArray2.size(), comparisonArray3.size(), comparisonArray5.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, comparisonArray.size(), comparisonArray5.size()));

                }
            }

        }

        return 111; //something went wrong if this value returns
    }
    else if (f1+f2+f3+f4+f5 == 5)
    {
        Eng1Time(attempt, islong);
        Eng2Time(attempt, islong);
        Eng3Time(attempt, islong);
        Eng4Time(attempt, islong);
        Eng5Time(attempt, islong);
        //compare5();
        int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, comparisonArray.size(), comparisonArray2.size(), comparisonArray3.size());
        if(c1 != -1)
        {return c1;}
        else
        {
            c1 = compare3(comparisonArray, comparisonArray4, comparisonArray5, comparisonArray.size(), comparisonArray4.size(), comparisonArray5.size());
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray4, comparisonArray5, comparisonArray2.size(), comparisonArray4.size(), comparisonArray5.size());
                if(c1 != -1)
                {return c1;}
                else
                {
                    return (compare3(comparisonArray3, comparisonArray4, comparisonArray5, comparisonArray3.size(), comparisonArray4.size(), comparisonArray5.size()));


                }

            }
        }
    }

    return 111; //something went wrong if this returns
}

int MainWindow::trackLength(int trackNum)
{

    //function that returns length of a certain track piece. Current lengths are dummy lengths. This is a lookup table.
    if (trackNum == 0)
        return 2;
    if (trackNum == 1)
        return 2;
    if (trackNum == 2)
        return 2;
    if (trackNum == 3)
        return 2;
    if (trackNum == 4)
        return 2;
    if (trackNum == 5)
        return 2;
    if (trackNum == 6)
        return 2;
    if (trackNum == 7)
        return 2;
    if (trackNum == 8)
        return 2;
    if (trackNum == 9)
        return 2;
    if (trackNum == 10)
        return 2;
    if (trackNum == 11)
        return 2;
    if (trackNum == 12)
        return 2;
    if (trackNum == 13)
        return 2;
    if (trackNum == 14)
        return 2;
    if (trackNum == 15)
        return 2;
    if (trackNum == 16)
        return 2;
    if (trackNum == 17)
        return 2;
    if (trackNum == 18)
        return 2;
    if (trackNum == 19)
        return 2;
    if (trackNum == 20)
        return 2;
    if (trackNum == 21)
        return 2;
    if (trackNum == 22)
        return 2;
    if (trackNum == 23)
        return 2;
    if (trackNum == 24)
        return 2;
    if (trackNum == 25)
        return 2;
    if (trackNum == 26)
        return 2;
    if (trackNum == 27)
        return 2;
    if (trackNum == 28)
        return 2;
    if (trackNum == 29)
        return 2;
    if (trackNum == 30)
        return 2;
    if (trackNum == 31)
        return 2;
    if (trackNum == 32)
        return 2;
    if (trackNum == 33)
        return 2;
    if (trackNum == 34)
        return 2;
    if (trackNum == 35)
        return 2;
    if (trackNum == 36)
        return 2;
    if (trackNum == 37)
        return 2;
    if (trackNum == 38)
        return 2;
    if (trackNum == 39)
        return 2;
    if (trackNum == 40)
        return 2;
    if (trackNum == 41)
        return 2;
    if (trackNum == 42)
        return 2;
    if (trackNum == 43)
        return 2;
    if (trackNum == 44)
        return 2;
    if (trackNum == 45)
        return 2;
    if (trackNum == 46)
        return 2;
    if (trackNum == 47)
        return 2;
    if (trackNum == 48)
        return 2;
    if (trackNum == 49)
        return 2;
    if (trackNum == 50)
        return 2;
    if (trackNum == 51)
        return 2;
    if (trackNum == 52)
        return 2;
    if (trackNum == 53)
        return 2;
    if (trackNum == 54)
        return 2;
    if (trackNum == 55)
        return 2;
    if (trackNum == 56)
        return 2;
    if (trackNum == 57)
        return 2;
    if (trackNum == 58)
        return 2;
    if (trackNum == 59)
        return 2;
    if (trackNum == 60)
        return 2;
    if (trackNum == 61)
        return 2;
    if (trackNum == 62)
        return 2;
    if (trackNum == 63)
        return 2;
    if (trackNum == 64)
        return 2;
    if (trackNum == 65)
        return 2;
    if (trackNum == 66)
        return 2;
    if (trackNum == 67)
        return 2;
    if (trackNum == 68)
        return 2;
    if (trackNum == 69)
        return 2;
    if (trackNum == 70)
        return 2;
    if (trackNum == 71)
        return 2;
    if (trackNum == 72)
        return 2;
    if (trackNum == 73)
        return 2;
    if (trackNum == 74)
        return 2;
    if (trackNum == 75)
        return 2;
    if (trackNum == 76)
        return 2;
    if (trackNum == 77)
        return 2;
    if (trackNum == 78)
        return 2;
    if (trackNum == 79)
        return 2;
    if (trackNum == 80)
        return 2;
    if (trackNum == 81)
        return 2;
    if (trackNum == 82)
        return 2;
    if (trackNum == 83)
        return 2;
    if (trackNum == 84)
        return 2;
    if (trackNum == 85)
        return 2;
    if (trackNum == 86)
        return 2;
    if (trackNum == 87)
        return 2;
    if (trackNum == 88)
        return 2;
    if (trackNum == 89)
        return 2;
    if (trackNum == 90)
        return 2;
    if (trackNum == 91)
        return 2;
    if (trackNum == 92)
        return 2;
    if (trackNum == 93)
        return 2;
    if (trackNum == 94)
        return 2;
    if (trackNum == 95)
        return 2;
    if (trackNum == 96)
        return 2;
    if (trackNum == 97)
        return 2;
    if (trackNum == 98)
        return 2;
    if (trackNum == 99)
        return 2;
    else
        return 2;
}

void MainWindow::Eng1Time( int attempt, int islong)
{

    //This function takes the route given by Dijkstras algorithm, and calculates how long it will take to cross each individial piece of track. It uses throttle, train speed, and track length.
    
    if (islong == 0){

    comparisonArray.clear();} //If this isn't a continuation path.

    float throttle1 =(ui->throttleBox1->currentText().toFloat()/100);
    float len = 0; //length * speed of current piece of track
    float overflow = 0;
    int j = 0;
    int i = 0;
    if (attempt == 1 || attempt == 2)
       { i++;}
    for (; i<E1S; i++)
    {
        
        //This block of code handles float to int conversion. For example, if it takes slightly more than a second to cross a track piece, then rounding down will eventually get you off course.
        //This function calculates the extra bits and saves them, so that when the fractions of a second add up to a whole second, the output accounts for this.
        double intPart;
        len = trackLength(Eng1[i]) * Eng1Speed / throttle1;
        double fracPart = modf(len, &intPart);
        overflow = overflow + fracPart;
        
        
        if( overflow >= 1)
        {
            overflow = overflow - 1;
            for(int k=0; k<(intPart + 1); k++)
            {
                comparisonArray.push_back(Eng1[i]);
                j++;
            }
        }
        else
        {
            for(int k=0; k<intPart; k++)
            {
                comparisonArray.push_back(Eng1[i]);
                j++;
            }
        }
    }

    //The following code is the delay code. If two trains are leaving the same end siding area, then you account for the delay.
    
    if(Eng1[0] == 53 || Eng1[0] == 54 || Eng1[0] == 55)
    {

    if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    }
    else if(Eng1[0] == 44 || Eng1[0] == 43)
    {

    if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44 ))
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    }

}

void MainWindow::Eng2Time(int attempt, int islong)
{
//This is the same function as Eng1Time, only changed for Eng2's variables.

    if (islong == 0){


    comparisonArray2.clear();
    }
    float throttle2 =(ui->throttleBox2->currentText().toFloat()/100);
    float len = 0; //length * speed of current piece of track
    float overflow = 0;
    int j = 0;
    int i = 0;
    if (attempt == 1 || attempt == 2)
       { i++;}
    for (; i<E2S; i++)
    {
        double intPart;
        len = trackLength(Eng2[i]) * Eng2Speed / throttle2;
        double fracPart = modf(len, &intPart);
        overflow = overflow + fracPart;
        if( overflow >= 1)
        {
            overflow = overflow - 1;
            for(int k=0; k<(intPart + 1); k++)
            {
                comparisonArray2.push_back(Eng2[i]);
                j++;
            }
        }
        else
        {
            for(int k=0; k<intPart; k++)
            {
                comparisonArray2.push_back(Eng2[i]);
                j++;
            }
        }
    }

    if(Eng2[0] == 53 || Eng2[0] == 54 || Eng2[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    }
    else if(Eng2[0] == 44 || Eng2[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    }


}

void MainWindow::Eng3Time(int attempt, int islong)
{
    if (islong == 0){

//        if (pathcomplete3 == 1)
//        {
//            return; //path is already being output. Do not recalculate
//        }
    comparisonArray3.clear();
    }
    float throttle3 =(ui->throttleBox3->currentText().toFloat()/100);
    float len = 0; //length * speed of current piece of track
    int j = 0;
    float overflow = 0;
    int i = 0;
    if (attempt == 1 || attempt == 2)
       { i++;}
    for (; i<E3S; i++)
    {
        double intPart;
        len = trackLength(Eng3[i]) * Eng3Speed / throttle3;
        double fracPart = modf(len, &intPart);
        overflow = overflow + fracPart;
        if( overflow >= 1)
        {
            overflow = overflow - 1;
            for(int k=0; k<(intPart + 1); k++)
            {
                comparisonArray3.push_back(Eng3[i]);
                j++;
            }
        }
        else
        {
            for(int k=0; k<intPart; k++)
            {
                comparisonArray3.push_back(Eng3[i]);
                j++;
            }
        }
    }

    if(Eng3[0] == 53 || Eng3[0] == 54 || Eng3[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    }
    else if(Eng3[0] == 44 || Eng3[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    }

}

void MainWindow::Eng4Time(int attempt, int islong)
{
    if (islong == 0){

//        if (pathcomplete4 == 1)
//        {
//            return; //path is already being output. Do not recalculate
//        }
    comparisonArray4.clear();
    }
    float throttle4 =(ui->throttleBox4->currentText().toFloat()/100);
    float len = 0; //length * speed of current piece of track
    int j = 0;
    float overflow = 0;
    int i = 0;
    if (attempt == 1 || attempt == 2)
       { i++;}
    for (; i<E4S; i++)
    {
        double intPart;
        len = trackLength(Eng4[i]) * Eng4Speed / throttle4;
        double fracPart = modf(len, &intPart);
        overflow = overflow + fracPart;
        if( overflow >= 1)
        {
            overflow = overflow - 1;
            for(int k=0; k<(intPart + 1); k++)
            {
                comparisonArray4.push_back(Eng4[i]);
                j++;
            }
        }
        else
        {
            for(int k=0; k<intPart; k++)
            {
                comparisonArray4.push_back(Eng4[i]);
                j++;
            }
        }
    }

    if(Eng4[0] == 53 || Eng4[0] == 54 || Eng4[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    }
    else if(Eng4[0] == 44 || Eng4[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    }

}

void MainWindow::Eng5Time(int attempt, int islong)
{
    if (islong == 0){

//        if (pathcomplete5 == 1)
//        {
//            return; //path is already being output. Do not recalculate
//        }
    comparisonArray5.clear();
    }
    float throttle5 =(ui->throttleBox5->currentText().toFloat()/100);
    float len = 0; //length * speed of current piece of track
    float overflow = 0;
    int j = 0;
    int i = 0;
    if (attempt == 1 || attempt == 2)
       { i++;}
    for (; i<E5S; i++)
    {
        double intPart;
        len = trackLength(Eng5[i]) * Eng5Speed / throttle5;
        double fracPart = modf(len, &intPart);
        overflow = overflow + fracPart;
        if( overflow >= 1)
        {
            overflow = overflow - 1;
            for(int k=0; k<(intPart + 1); k++)
            {
                comparisonArray5.push_back(Eng5[i]);
                j++;
            }
        }
        else
        {
            for(int k=0; k<intPart; k++)
            {
                comparisonArray5.push_back(Eng5[i]);
                j++;
            }
        }
    }
    if(Eng5[0] == 53 || Eng5[0] == 54 || Eng5[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    }
    else if(Eng5[0] == 44 || Eng5[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<10; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    }
}

int MainWindow::compare2(std::vector<int>  a, std::vector<int>  b, int c, int d)
{

    //This function detects collisions. It compares the arrays it is passed, and if there is a collision, it returns the segment of the collision.
    

    if (c >= d ) //Determine which array is shorter, and compare that many values. Once a train is stopped on an end siding, we don't care about it any more.
    {

      for(int i=0; i<d; i++)
      {
          if (a[i] == b[i])
          {
              cout << "Collision detected at track segment " << oursTotheirs(a[i]).toStdString() << endl;
              return a[i];
          }

      }
    }
    else
    {
        for(int i=0; i<c; i++)
        {
            if(a[i] == b[i])
            {
                cout << "Collision detected at track segment " << oursTotheirs(a[i]).toStdString() << endl;
                return a[i];
            }
        }
    }

    return -1;
}


int MainWindow::compare3(std::vector<int>  a, std::vector<int>  b, std::vector<int>  c, int a1, int b1, int c1)
{
    //This function does the same thing compare2 does, only with 3 values. 
int i = 0;
    if(a1 >= b1 && b1 >= c1)
    {
        for (; i<c1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i]) //Determine which array is shorter, and compare that many values. Once a train is stopped on an end siding, we don't care about it any more.
         {

                     if(a[i] == b[i] || a[i] == c[i]) //If trains intersect.
                     {
                         cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                             return a[i];
                     }
                     else{
                      cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                            return b[i];
                     }
         }
        }
        for(;i<b1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }
    }


    else if(a1 >= c1 && c1 >= b1)
    {
        for (; i<b1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {

                     if(a[i] == b[i] || a[i] == c[i]){
                            cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                         return a[i];
                     }
                     else{
                         cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                            return b[i];}
         }
        }
        for(;i<c1;i++)
        {
            if(a[i]==c[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }

    }


    else if(b1 >= a1 && a1 >= c1)
    {
        for (; i<c1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {

                     if(a[i] == b[i] || a[i] == c[i]){

                     cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                     return a[i];
                     }
                     else{

                     cout << "Paths intersect at track section " << oursTotheirs(b[i]).toStdString()  << endl;
                     return b[i];}
         }
        }
        for(;i<a1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }
    }


    else if(b1 >= c1 && c1 >= a1)
    {
        for (; i<a1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {

                     if(a[i] == b[i] || a[i] == c[i])
                             {
                         cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                         return a[i];
                     }
                     else{
                            return b[i];
                     cout << "Paths intersect at track section " << oursTotheirs(b[i]).toStdString()  << endl;
                     }
         }
        }
        for(;i<c1;i++)
        {
            if(c[i]==b[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }
    }


    else if(c1 >= a1 && a1 >= b1)
    {
        for (; i<b1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {

                     if(a[i] == b[i] || a[i] == c[i])
                     {
                         cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                         return a[i];
                     }
                     else
                     {
                         cout << "Paths intersect at track section " << oursTotheirs(b[i]).toStdString()  << endl;
                            return b[i];
         }
         }
        }
        for(;i<a1;i++)
        {
            if(a[i]==c[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }
    }


    else if(c1 >=b1 && b1 >=a1)
    {

        for (; i<a1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {

                     if(a[i] == b[i] || a[i] == c[i])
                     {
                         cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                             return a[i];
                     }
                      else
                     {
                         cout << "Paths intersect at track section " << oursTotheirs(b[i]).toStdString()  << endl;
                            return b[i];
         }
         }
        }
        for(;i<b1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect at track section " << oursTotheirs(a[i]).toStdString()  << endl;
                return a[i];
            }
        }
    }
    return -1;
}


void MainWindow::reroute(int trainSel, int tVal)
{

    //The reroute code goes in and remakes the adjacency list, leaving out the segment that resulted in a crash. The train is then rerouted through a different node, if possible.
    
    adjacency_list_t adjacency_list(100);

    if(tVal != 0)
    {
        adjacency_list[0].push_back(neighbor(1, 2));
        adjacency_list[0].push_back(neighbor(2, 2));
        adjacency_list[0].push_back(neighbor(39, 2));
    }
    if(tVal != 1)
    {
        adjacency_list[1].push_back(neighbor(81, 2));
        adjacency_list[1].push_back(neighbor(0, 2));
    }
    if(tVal != 2)
    {
         adjacency_list[2].push_back(neighbor(71, 2));
         adjacency_list[2].push_back(neighbor(0, 2));
         adjacency_list[2].push_back(neighbor(4, 2));
    }
    if(tVal != 3)
    {
        adjacency_list[3].push_back(neighbor(81, 2));
        adjacency_list[3].push_back(neighbor(5, 2));
    }
    if(tVal != 4)
    {
        adjacency_list[4].push_back(neighbor(2, 2));
        adjacency_list[4].push_back(neighbor(6, 2));
        adjacency_list[4].push_back(neighbor(5, 2));
    }
    if(tVal != 5)
    {
        adjacency_list[5].push_back(neighbor(7, 2));
        adjacency_list[5].push_back(neighbor(3, 2));
    }
    if(tVal != 6)
    {
        adjacency_list[6].push_back(neighbor(4, 2));
    }
    if(tVal != 7)
    {
        adjacency_list[7].push_back(neighbor(8, 2));
        adjacency_list[7].push_back(neighbor(9, 2));
        adjacency_list[7].push_back(neighbor(5, 2));
    }
    if(tVal != 8)
    {
        adjacency_list[8].push_back(neighbor(80, 2));
        adjacency_list[8].push_back(neighbor(7, 2));
    }
    if(tVal != 9)
    {
        adjacency_list[9].push_back(neighbor(7, 2));
        adjacency_list[9].push_back(neighbor(98, 2));
    }
    if(tVal != 10)
    {
        adjacency_list[10].push_back(neighbor(11, 2));
        adjacency_list[10].push_back(neighbor(12, 2));
        adjacency_list[10].push_back(neighbor(98, 2));
    }
    if(tVal != 11)
    {
        adjacency_list[11].push_back(neighbor(10, 2));
        adjacency_list[11].push_back(neighbor(13, 2));
    }
    if(tVal != 12)
    {
        adjacency_list[12].push_back(neighbor(82, 2));
        adjacency_list[12].push_back(neighbor(10, 2));
    }
    if(tVal != 13)
    {
        adjacency_list[13].push_back(neighbor(11, 2));
    }
    if(tVal != 14)
    {
        adjacency_list[14].push_back(neighbor(15, 2));
        adjacency_list[14].push_back(neighbor(16, 2));
        adjacency_list[14].push_back(neighbor(79, 2));
    }
    if(tVal != 15)
    {
        adjacency_list[15].push_back(neighbor(40, 2));
        adjacency_list[15].push_back(neighbor(14, 2));
    }
    if(tVal != 16)
    {
        adjacency_list[16].push_back(neighbor(17, 2));
        adjacency_list[16].push_back(neighbor(82, 2));
        adjacency_list[16].push_back(neighbor(14, 2));
    }
    if(tVal != 17)
    {
        adjacency_list[17].push_back(neighbor(19, 2));
        adjacency_list[17].push_back(neighbor(16, 2));
        adjacency_list[17].push_back(neighbor(18, 2));
    }
    if(tVal != 18)
    {
        adjacency_list[18].push_back(neighbor(17, 2));
        adjacency_list[18].push_back(neighbor(20, 2));
    }
    if(tVal != 19)
    {
        adjacency_list[19].push_back(neighbor(6, 2));
        adjacency_list[19].push_back(neighbor(17, 2));
    }
    if(tVal != 20)
    {
        adjacency_list[20].push_back(neighbor(18, 2));
        adjacency_list[20].push_back(neighbor(22, 2));
        adjacency_list[20].push_back(neighbor(21, 2));
    }
    if(tVal != 21)
    {
        adjacency_list[21].push_back(neighbor(20, 2));
        adjacency_list[21].push_back(neighbor(83, 2));
    }
    if(tVal != 22)
    {
        adjacency_list[22].push_back(neighbor(24, 2));
        adjacency_list[22].push_back(neighbor(20, 2));
        adjacency_list[22].push_back(neighbor(23, 2));
    }
    if(tVal != 23)
    {
        adjacency_list[23].push_back(neighbor(22, 2));
        adjacency_list[23].push_back(neighbor(27, 2));
    }
    if(tVal != 24)
    {
        adjacency_list[24].push_back(neighbor(22, 2));
        adjacency_list[24].push_back(neighbor(25, 2));
    }
    if(tVal != 25)
    {
        adjacency_list[25].push_back(neighbor(24, 2));
        adjacency_list[25].push_back(neighbor(26, 2));
    }
    if(tVal != 26)
    {
        adjacency_list[26].push_back(neighbor(28, 2));
        adjacency_list[26].push_back(neighbor(51, 2));
        adjacency_list[26].push_back(neighbor(25, 2));
    }
    if(tVal != 27)
    {
        adjacency_list[27].push_back(neighbor(23, 2));
        adjacency_list[27].push_back(neighbor(28, 2));
    }
    if(tVal != 28)
    {
        adjacency_list[28].push_back(neighbor(29, 2));
        adjacency_list[28].push_back(neighbor(26, 2));
        adjacency_list[28].push_back(neighbor(27, 2));
    }
    if(tVal != 29)
    {
        adjacency_list[29].push_back(neighbor(84, 2));
        adjacency_list[29].push_back(neighbor(28, 2));
        adjacency_list[29].push_back(neighbor(30, 2));
    }
    if(tVal != 30)
    {
        adjacency_list[30].push_back(neighbor(31, 2));
        adjacency_list[30].push_back(neighbor(29, 2));
    }
    if(tVal != 31)
    {
        adjacency_list[31].push_back(neighbor(30, 2));
        adjacency_list[31].push_back(neighbor(32, 2));
        adjacency_list[31].push_back(neighbor(70, 2));
    }
    if(tVal != 32)
    {
        adjacency_list[32].push_back(neighbor(34, 2));
        adjacency_list[32].push_back(neighbor(33, 2));
        adjacency_list[32].push_back(neighbor(31, 2));
    }
    if(tVal != 33)
    {
        adjacency_list[33].push_back(neighbor(32, 2));
        adjacency_list[33].push_back(neighbor(90, 2));
    }
    if(tVal != 34)
    {
        adjacency_list[34].push_back(neighbor(89, 2));
        adjacency_list[34].push_back(neighbor(50, 2));
        adjacency_list[34].push_back(neighbor(32, 2));
    }
    if(tVal != 35)
    {
        adjacency_list[35].push_back(neighbor(90, 2));
        adjacency_list[35].push_back(neighbor(36, 2));
    }
    if(tVal != 36)
    {
        adjacency_list[36].push_back(neighbor(35, 2));
        adjacency_list[36].push_back(neighbor(68, 2));
        adjacency_list[36].push_back(neighbor(91, 2));
    }
    if(tVal != 37)
    {
        adjacency_list[37].push_back(neighbor(39, 2));
        adjacency_list[37].push_back(neighbor(88, 2));
    }
    if(tVal != 38)
    {
        adjacency_list[38].push_back(neighbor(39, 2));
        adjacency_list[38].push_back(neighbor(91, 2));
    }
    if(tVal != 39)
    {
        adjacency_list[39].push_back(neighbor(0, 2));
        adjacency_list[39].push_back(neighbor(38, 2));
        adjacency_list[39].push_back(neighbor(37, 2));
    }
    if(tVal != 40)
    {
        adjacency_list[40].push_back(neighbor(15, 2));
        adjacency_list[40].push_back(neighbor(41, 2));
        adjacency_list[40].push_back(neighbor(45, 2));
    }
    if(tVal != 41)
    {
        adjacency_list[41].push_back(neighbor(46, 2));
        adjacency_list[41].push_back(neighbor(40, 2));
        adjacency_list[41].push_back(neighbor(42, 2));
    }
    if(tVal != 42)
    {
        adjacency_list[42].push_back(neighbor(75, 2));
        adjacency_list[42].push_back(neighbor(76, 2));
        adjacency_list[42].push_back(neighbor(41, 2));
    }
    if(tVal != 43)
    {
        adjacency_list[43].push_back(neighbor(75, 2));
    }
    if(tVal != 44)
    {
        adjacency_list[44].push_back(neighbor(76, 2));

    }
    if(tVal != 45)
    {
        adjacency_list[45].push_back(neighbor(40, 2));
        adjacency_list[45].push_back(neighbor(78, 2));
    }
    if(tVal != 46)
    {
        adjacency_list[46].push_back(neighbor(41, 2));
        adjacency_list[46].push_back(neighbor(77, 2));
    }
    if(tVal != 47)
    {
        adjacency_list[47].push_back(neighbor(78, 2));
        adjacency_list[47].push_back(neighbor(49, 2));
    }
    if(tVal != 48)
    {
        adjacency_list[48].push_back(neighbor(77, 2));
        adjacency_list[48].push_back(neighbor(49, 2));
    }
    if(tVal != 49)
    {
        adjacency_list[49].push_back(neighbor(47, 2));
        adjacency_list[49].push_back(neighbor(48, 2));
        adjacency_list[49].push_back(neighbor(50, 2));

    }
    if(tVal != 50)
    {
        adjacency_list[50].push_back(neighbor(34, 2));
        adjacency_list[50].push_back(neighbor(49, 2));
    }
    if(tVal != 51)
    {
        adjacency_list[51].push_back(neighbor(26, 2));
        adjacency_list[51].push_back(neighbor(52, 2));
        adjacency_list[51].push_back(neighbor(87, 2));
    }
    if(tVal != 52)
    {
        adjacency_list[52].push_back(neighbor(51, 2));
        adjacency_list[52].push_back(neighbor(85, 2));
        adjacency_list[52].push_back(neighbor(86, 2));
    }
    if(tVal != 53)
    {
        adjacency_list[53].push_back(neighbor(85, 2));

    }
    if(tVal != 54)
    {
        adjacency_list[54].push_back(neighbor(86, 2));

    }
    if(tVal != 55)
    {
        adjacency_list[55].push_back(neighbor(87, 2));

    }
    if(tVal != 68)
    {
        adjacency_list[68].push_back(neighbor(36, 2));
        adjacency_list[68].push_back(neighbor(69, 2));
    }
    if(tVal != 69)
    {
        adjacency_list[69].push_back(neighbor(68, 2));
    }
    if(tVal != 70)
    {
        adjacency_list[70].push_back(neighbor(31, 2));
        adjacency_list[70].push_back(neighbor(71, 2));
    }
    if(tVal != 71)
    {
        adjacency_list[71].push_back(neighbor(70, 2));
        adjacency_list[71].push_back(neighbor(2, 2));
    }
    if(tVal != 75)
    {
        adjacency_list[75].push_back(neighbor(43, 2));
        adjacency_list[75].push_back(neighbor(42, 2));
    }
    if(tVal != 76)
    {
        adjacency_list[76].push_back(neighbor(42, 2));
        adjacency_list[76].push_back(neighbor(44, 2));
    }
    if(tVal != 77)
    {
        adjacency_list[77].push_back(neighbor(46, 2));
        adjacency_list[77].push_back(neighbor(48, 2));
    }
    if(tVal != 78)
    {
        adjacency_list[78].push_back(neighbor(45, 2));
        adjacency_list[78].push_back(neighbor(47, 2));
    }
    if(tVal != 79)
    {
        adjacency_list[79].push_back(neighbor(14, 2));
        adjacency_list[79].push_back(neighbor(80, 2));
    }
    if(tVal != 80)
    {
        adjacency_list[80].push_back(neighbor(79, 2));
        adjacency_list[80].push_back(neighbor(8, 2));
    }
    if(tVal != 81)
    {
        adjacency_list[81].push_back(neighbor(3, 2));
        adjacency_list[81].push_back(neighbor(1, 2));
    }
    if(tVal != 82)
    {
        adjacency_list[82].push_back(neighbor(16, 2));
        adjacency_list[82].push_back(neighbor(12, 2));
    }
    if(tVal != 83)
    {
        adjacency_list[83].push_back(neighbor(21, 2));
        adjacency_list[83].push_back(neighbor(84, 2));
    }
    if(tVal != 84)
    {
        adjacency_list[84].push_back(neighbor(83, 2));
        adjacency_list[84].push_back(neighbor(29, 2));
    }
    if(tVal != 85)
    {
        adjacency_list[85].push_back(neighbor(52, 2));
        adjacency_list[85].push_back(neighbor(53, 2));
    }
    if(tVal != 86)
    {
        adjacency_list[86].push_back(neighbor(52, 2));
        adjacency_list[86].push_back(neighbor(54, 2));
    }
    if(tVal != 87)
    {
        adjacency_list[87].push_back(neighbor(51, 2));
        adjacency_list[87].push_back(neighbor(55, 2));
    }
    if(tVal != 88)
    {
        adjacency_list[88].push_back(neighbor(37, 2));
        adjacency_list[88].push_back(neighbor(89, 2));
    }
    if(tVal != 89)
    {
        adjacency_list[89].push_back(neighbor(88, 2));
        adjacency_list[89].push_back(neighbor(34, 2));
    }
    if(tVal != 90)
    {
        adjacency_list[90].push_back(neighbor(33, 2));
        adjacency_list[90].push_back(neighbor(35, 2));
    }
    if(tVal != 91)
    {
        adjacency_list[91].push_back(neighbor(38, 2));
        adjacency_list[91].push_back(neighbor(36, 2));
    }

    if(tVal != 98)
    {
        adjacency_list[98].push_back(neighbor(10, 2));
        adjacency_list[98].push_back(neighbor(9, 2));
    }



    //unused: 65-67, 72-74, 92-97, 99

    int start;
    int end;
if(trainSel == 1) //if train1 is the active train
{
    //translate origin box
    if (ui->originBox1->currentIndex() == 1)
        start = 43;
    else if (ui->originBox1->currentIndex() == 2)
        start = 44;
    else if (ui->originBox1->currentIndex() == 3)
        start = 55;
    else if (ui->originBox1->currentIndex() == 4)
        start = 54;
    else if (ui->originBox1->currentIndex() == 5)
        start = 53;
    else if (ui->originBox1->currentIndex() == 6)
        start = 13;
    else if (ui->originBox1->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox1->currentIndex() == 1)
        end = 43;
    else if (ui->destBox1->currentIndex() == 2)
        end = 44;
    else if (ui->destBox1->currentIndex() == 3)
        end = 55;
    else if (ui->destBox1->currentIndex() == 4)
        end = 54;
    else if (ui->destBox1->currentIndex() == 5)
        end = 53;
    else if (ui->destBox1->currentIndex() == 6)
        end = 13;
    else if (ui->destBox1->currentIndex() == 7)
        end = 69;
}
else if(trainSel == 2) //if train 2 is the active train
{
    //translate origin box
    if (ui->originBox2->currentIndex() == 1)
        start = 43;
    else if (ui->originBox2->currentIndex() == 2)
        start = 44;
    else if (ui->originBox2->currentIndex() == 3)
        start = 55;
    else if (ui->originBox2->currentIndex() == 4)
        start = 54;
    else if (ui->originBox2->currentIndex() == 5)
        start = 53;
    else if (ui->originBox2->currentIndex() == 6)
        start = 13;
    else if (ui->originBox2->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox2->currentIndex() == 1)
        end = 43;
    else if (ui->destBox2->currentIndex() == 2)
        end = 44;
    else if (ui->destBox2->currentIndex() == 3)
        end = 55;
    else if (ui->destBox2->currentIndex() == 4)
        end = 54;
    else if (ui->destBox2->currentIndex() == 5)
        end = 53;
    else if (ui->destBox2->currentIndex() == 6)
        end = 13;
    else if (ui->destBox2->currentIndex() == 7)
        end = 69;
}
else if(trainSel == 3)
{
    //translate origin box
    if (ui->originBox3->currentIndex() == 1)
        start = 43;
    else if (ui->originBox3->currentIndex() == 2)
        start = 44;
    else if (ui->originBox3->currentIndex() == 3)
        start = 55;
    else if (ui->originBox3->currentIndex() == 4)
        start = 54;
    else if (ui->originBox3->currentIndex() == 5)
        start = 53;
    else if (ui->originBox3->currentIndex() == 6)
        start = 13;
    else if (ui->originBox3->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox3->currentIndex() == 1)
        end = 43;
    else if (ui->destBox3->currentIndex() == 2)
        end = 44;
    else if (ui->destBox3->currentIndex() == 3)
        end = 55;
    else if (ui->destBox3->currentIndex() == 4)
        end = 54;
    else if (ui->destBox3->currentIndex() == 5)
        end = 53;
    else if (ui->destBox3->currentIndex() == 6)
        end = 13;
    else if (ui->destBox3->currentIndex() == 7)
        end = 69;
}
else if(trainSel == 4)
{
    //translate origin box
    if (ui->originBox4->currentIndex() == 1)
        start = 43;
    else if (ui->originBox4->currentIndex() == 2)
        start = 44;
    else if (ui->originBox4->currentIndex() == 3)
        start = 55;
    else if (ui->originBox4->currentIndex() == 4)
        start = 54;
    else if (ui->originBox4->currentIndex() == 5)
        start = 53;
    else if (ui->originBox4->currentIndex() == 6)
        start = 13;
    else if (ui->originBox4->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox4->currentIndex() == 1)
        end = 43;
    else if (ui->destBox4->currentIndex() == 2)
        end = 44;
    else if (ui->destBox4->currentIndex() == 3)
        end = 55;
    else if (ui->destBox4->currentIndex() == 4)
        end = 54;
    else if (ui->destBox4->currentIndex() == 5)
        end = 53;
    else if (ui->destBox4->currentIndex() == 6)
        end = 13;
    else if (ui->destBox4->currentIndex() == 7)
        end = 69;
}
if(trainSel == 5)
{
    //translate origin box
    if (ui->originBox5->currentIndex() == 1)
        start = 43;
    else if (ui->originBox5->currentIndex() == 2)
        start = 44;
    else if (ui->originBox5->currentIndex() == 3)
        start = 55;
    else if (ui->originBox5->currentIndex() == 4)
        start = 54;
    else if (ui->originBox5->currentIndex() == 5)
        start = 53;
    else if (ui->originBox5->currentIndex() == 6)
        start = 13;
    else if (ui->originBox5->currentIndex() == 7)
        start = 69;
    //translate destination box
    if (ui->destBox5->currentIndex() == 1)
        end = 43;
    else if (ui->destBox5->currentIndex() == 2)
        end = 44;
    else if (ui->destBox5->currentIndex() == 3)
        end = 55;
    else if (ui->destBox5->currentIndex() == 4)
        end = 54;
    else if (ui->destBox5->currentIndex() == 5)
        end = 53;
    else if (ui->destBox5->currentIndex() == 6)
        end = 13;
    else if (ui->destBox5->currentIndex() == 7)
        end = 69;
}


//The following code basically reruns the code from the greyout functions in order to get a working path.

    std::vector<weight_t> min_distance;
    std::vector<vertex_t> previous;
    DijkstraComputePaths(start, adjacency_list, min_distance, previous);

    if (trainSel == 1)
    {
    std::cout << "Rerouted Distance from " << ui->originBox1->currentText().toStdString() << " to " << ui->destBox1->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
    backupPath = DijkstraGetShortestPathTo(end, previous);
    //std::cout << ui->trainselectBox1->currentText().toStdString() <<" Path : ";
    }
    else if (trainSel == 2)
    {
    std::cout << "Rerouted Distance from " << ui->originBox2->currentText().toStdString() << " to " << ui->destBox2->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
    backupPath = DijkstraGetShortestPathTo(end, previous);
    //std::cout << ui->trainselectBox2->currentText().toStdString() <<" Path : ";
    }
    else if (trainSel == 3)
    {
    std::cout << "Rerouted Distance from " << ui->originBox3->currentText().toStdString() << " to " << ui->destBox3->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
    backupPath = DijkstraGetShortestPathTo(end, previous);
    //std::cout << ui->trainselectBox3->currentText().toStdString() <<" Path : ";
    }
    else if (trainSel == 4)
    {
        std::cout << "Rerouted Distance from " << ui->originBox4->currentText().toStdString() << " to " << ui->destBox4->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
        backupPath = DijkstraGetShortestPathTo(end, previous);
       // std::cout << ui->trainselectBox4->currentText().toStdString() <<" Path : ";
    }
    else if (trainSel == 5)
    {
    std::cout << "Rerouted Distance from " << ui->originBox5->currentText().toStdString() << " to " << ui->destBox5->currentText().toStdString() <<  ": " << to_string(min_distance[end]) << std::endl;
    backupPath = DijkstraGetShortestPathTo(end, previous);
    //std::cout << ui->trainselectBox5->currentText().toStdString() <<" Path : ";
    }


}




#endif // CHECKPATHS_H
