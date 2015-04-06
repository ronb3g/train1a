#ifndef CHECKPATHS_H
#define CHECKPATHS_H
#include <mainwindow.h>
#include <tableOperations.h>



template <class T>
inline std::string to_string (const T& t)
{
std::stringstream ss;
ss << t;
return ss.str();
}


int MainWindow::checkPaths(int trainS)
{
    int f1 = 0;
    int f2 = 0;
    int f3 = 0;
    int f4 = 0;
    int f5 = 0;


    //Check to see which paths have been planned so far
    if ( Eng1 != NULL)
    {
        f1 = 1;
        comparisonArray = new int[i1];
    }
    if ( Eng2 != NULL)
    {
        f2 = 1;
        comparisonArray2 = new int[i2];
    }
    if ( Eng3 != NULL)
    {
        f3 = 1;
        comparisonArray3 = new int[i3];
    }
    if ( Eng4 != NULL)
    {
        f4 = 1;
        comparisonArray4 = new int[i4];
    }
    if ( Eng5 != NULL)
    {
        f5 = 1;
        comparisonArray5 = new int[i5];
    }

// delete comparisonArray;
 //delete comparisonArray2;
 //delete comparisonArray3;
 //delete comparisonArray4;
// delete comparisonArray5;
 //return -1;



    if ( (f1+f2+f3+f4+f5 == 1) || (f1+f2+f3+f4+f5 == 0))
    {
        //No paths or only one path detected. No comparison needed
        return -1;
    }
    else if (f1+f2+f3+f4+f5 == 2)
    { //2 paths detected. Determining possible collisions

        if(f1+f2 == 2)
        {

            Eng1Time();
            Eng2Time();
            return (compare2(comparisonArray, comparisonArray2, i1, i2));
        }
        else if (f1+f3 == 2)
        {
            Eng1Time();
            Eng3Time();
            return (compare2(comparisonArray, comparisonArray3, i1, i3));
        }
        else if (f1+f4 == 2)
        {
            Eng1Time();
            Eng4Time();
            return (compare2(comparisonArray,comparisonArray4, i1, i4));
        }
        else if(f1+f5 == 2)
        {
            Eng1Time();
            Eng5Time();
            return (compare2(comparisonArray,comparisonArray5,i1,i5));
        }
        else if(f2+f3 == 2)
        {
            Eng2Time();
            Eng3Time();
            return (compare2(comparisonArray2,comparisonArray3,i2,i3));
        }
        else if(f2+f4 == 2)
        {
            Eng2Time();
            Eng4Time();
            return (compare2(comparisonArray2,comparisonArray4,i2,i4));
        }
        else if(f2+f5 == 2)
        {
            Eng2Time();
            Eng5Time();
            return (compare2(comparisonArray2,comparisonArray5,i2,i5));
        }
        else if(f3+f4 == 2)
        {
            Eng3Time();
            Eng4Time();
            return (compare2(comparisonArray3,comparisonArray4,i3,i4));
        }
        else if(f3+f5 == 2)
        {
            Eng3Time();
            Eng5Time();
            return (compare2(comparisonArray3,comparisonArray5,i3,i5));
        }
        else if(f4+f5 == 2)
        {
            Eng4Time();
            Eng5Time();
            return (compare2(comparisonArray4,comparisonArray5,i4,i5));
        }
    }
    else if (f1+f2+f3+f4+f5 == 3)
      {// 3 paths detected. Checking for collisions.
        if(f1+f2+f3 == 3)
        {
            Eng1Time();
            Eng2Time();
            Eng3Time();
            return (compare3(comparisonArray,comparisonArray2,comparisonArray3,i1,i2,i3));
        }
        else if (f1+f2+f4 == 3)
        {
            Eng1Time();
            Eng2Time();
            Eng4Time();
            return (compare3(comparisonArray,comparisonArray2,comparisonArray4,i1,i2,i4));
        }
        else if (f1+f2+f5 == 3)
        {
            Eng1Time();
            Eng2Time();
            Eng5Time();
            return (compare3(comparisonArray,comparisonArray2,comparisonArray5,i1,i2,i5));
        }
        else if(f1+f3+f4 == 3)
        {
            Eng1Time();
            Eng3Time();
            Eng4Time();
            return (compare3(comparisonArray,comparisonArray3,comparisonArray4,i1,i3,i4));
        }
        else if(f1+f3+f5 == 3)
        {
            Eng1Time();
            Eng3Time();
            Eng5Time();
            return (compare3(comparisonArray,comparisonArray3,comparisonArray5,i1,i3,i5));
        }
        else if(f1+f4+f5 == 3)
        {
            Eng1Time();
            Eng4Time();
            Eng5Time();
            return (compare3(comparisonArray,comparisonArray4,comparisonArray5,i1,i4,i5));
        }
        else if(f2+f3+f4 == 3)
        {
            Eng2Time();
            Eng3Time();
            Eng4Time();
            return (compare3(comparisonArray2,comparisonArray3,comparisonArray4,i2,i3,i4));
        }
        else if(f2+f3+f5 == 3)
        {
            Eng2Time();
            Eng3Time();
            Eng5Time();
            return (compare3(comparisonArray2,comparisonArray3,comparisonArray5,i2,i3,i5));
        }
        else if(f2+f4+f5 == 3)
        {
            Eng2Time();
            Eng4Time();
            Eng5Time();
            return (compare3(comparisonArray2,comparisonArray4,comparisonArray5,i2,i4,i5));
        }
        else if(f3+f4+f5 == 3)
        {
            Eng3Time();
            Eng4Time();
            Eng5Time();
            return (compare3(comparisonArray3,comparisonArray4,comparisonArray5,i3,i4,i5));
        }
    }
    else if(f1+f2+f3+f4+f5 == 4)
    {//4 paths detected. checking now.
        if (f1+f2+f3+f4 == 4)
        {
            Eng1Time();
            Eng2Time();
            Eng3Time();
            Eng4Time();
            //compare4(comparisonArray,comparisonArray2,comparisonArray3,comparisonArray4,i1,i2,i3,i4);
            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, i1, i2, i3);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray4, i2, i3, i4);
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray4, i1, i4));

                }
            }


        }
        else if (f2+f3+f4+f5 == 4)
        {
            Eng5Time();
            Eng2Time();
            Eng3Time();
            Eng4Time();
            //compare4(comparisonArray2,comparisonArray3,comparisonArray4,comparisonArray5,i2,i3,i4,i5);
            int c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray4, i2, i3, i4);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray3, comparisonArray4, comparisonArray5, i3, i4, i5);
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray2, comparisonArray5, i2, i5));

                }
            }
        }
        else if (f3+f4+f5+f1 == 4)
        {
            Eng1Time();
            Eng5Time();
            Eng3Time();
            Eng4Time();
            //compare4(comparisonArray,comparisonArray3,comparisonArray4,comparisonArray5,i1,i3,i4,i5);
            int c1 = compare3(comparisonArray, comparisonArray4, comparisonArray3, i1, i4, i3);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray3, comparisonArray4, comparisonArray5, i3, i4, i5);
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, i1, i5));

                }
            }
        }
        else if(f4+f5+f1+f2 == 4)
        {
            Eng1Time();
            Eng2Time();
            Eng5Time();
            Eng4Time();
            //compare4(comparisonArray,comparisonArray2,comparisonArray4,comparisonArray5,i1,i2,i4,i5);
            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray4, i1, i2, i4);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray4, comparisonArray5, i2, i4, i5);
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, i1, i5));

                }
            }

        }
        else if (f1+f2+f3+f5 == 4)
        {
            Eng1Time();
            Eng2Time();
            Eng3Time();
            Eng5Time();
            //compare4(comparisonArray,comparisonArray2,comparisonArray3,comparisonArray5,i1,i2,i3,i5);

            int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, i1, i2, i3);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray3, comparisonArray5, i2, i3, i5);
                if(c1 != -1)
                {return c1;}
                else
                {
                   return (compare2(comparisonArray, comparisonArray5, i1, i5));

                }
            }

        }

        return 111; //something went wrong if this value returns
    }
    else if (f1+f2+f3+f4+f5 == 5)
    {
        Eng1Time();
        Eng2Time();
        Eng3Time();
        Eng4Time();
        Eng5Time();
        //compare5();
        int c1 = compare3(comparisonArray, comparisonArray2, comparisonArray3, i1, i2, i3);
        if(c1 != -1)
        {return c1;}
        else
        {
            c1 = compare3(comparisonArray, comparisonArray4, comparisonArray5, i1, i4, i5);
            if(c1 != -1)
            {return c1;}
            else
            {
                c1 = compare3(comparisonArray2, comparisonArray4, comparisonArray5, i2, i4, i5);
                if(c1 != -1)
                {return c1;}
                else
                {
                    return (compare3(comparisonArray3, comparisonArray4, comparisonArray5, i3, i4, i5));


                }

            }
        }
    }

    return 111; //something went wrong if this returns
}

int MainWindow::trackLength(int trackNum)
{

    //function that returns length of a certain track piece. Current lengths are dummy lengths.
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

void MainWindow::Eng1Time()
{

    comparisonArray.clear();
    int len = 0; //length * speed of current piece of track
    int j = 0;
    for (int i=0; i<E1S; i++)
    {
       len = trackLength(Eng1[i]) * Eng1Speed;
        for(int k=0; k<len; k++)
        {
           comparisonArray.push_back(Eng1[i]);
            j++;
        }
    }

    if(Eng1[0] == 53 || Eng1[0] == 54 || Eng1[0] == 55)
    {

    if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    }
    else if(Eng1[0] == 44 || Eng1[0] == 43)
    {

    if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44 ))
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray.insert(comparisonArray.begin(), Eng1[0]);
        }
    }
    }

}

void MainWindow::Eng2Time()
{
    comparisonArray2.clear();
    int len = 0; //length * speed of current piece of track
    int j = 0;
    for (int i=0; i<E2S; i++)
    {
        len = trackLength(Eng2[i]) * Eng2Speed;
        for(int k=0; k<len; k++)
        {
            comparisonArray2.push_back(Eng2[i]);
            j++;
        }
    }

    if(Eng2[0] == 53 || Eng2[0] == 54 || Eng2[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    }
    else if(Eng2[0] == 44 || Eng2[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray2.insert(comparisonArray2.begin(), Eng2[0]);
        }
    }
    }

}

void MainWindow::Eng3Time()
{
    comparisonArray3.clear();
    int len = 0; //length * speed of current piece of track
    int j = 0;
    for (int i=0; i<E3S; i++)
    {
        len = trackLength(Eng3[i]) * Eng3Speed;
        for(int k=0; k<len; k++)
        {
            comparisonArray3.push_back(Eng3[i]);
            j++;
        }
    }

    if(Eng3[0] == 53 || Eng3[0] == 54 || Eng3[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    }
    else if(Eng3[0] == 44 || Eng3[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray3.insert(comparisonArray3.begin(), Eng3[0]);
        }
    }
    }

}

void MainWindow::Eng4Time()
{
    comparisonArray4.clear();
    int len = 0; //length * speed of current piece of track
    int j = 0;
    for (int i=0; i<E4S; i++)
    {
        len = trackLength(Eng4[i]) * Eng4Speed;
        for(int k=0; k<len; k++)
        {
            comparisonArray4.push_back(Eng4[i]);
            j++;
        }
    }

    if(Eng4[0] == 53 || Eng4[0] == 54 || Eng4[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 53 || comparisonArray5[0] == 54 || comparisonArray5[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    }
    else if(Eng5[0] == 44 || Eng5[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    else if(comparisonArray5.size() !=0 && ( comparisonArray5[0] == 43 || comparisonArray5[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray4.insert(comparisonArray4.begin(), Eng4[0]);
        }
    }
    }

}

void MainWindow::Eng5Time()
{
    comparisonArray5.clear();
    int len = 0; //length * speed of current piece of track
    int j = 0;
    for (int i=0; i<E5S; i++)
    {
        len = trackLength(Eng5[i]) * Eng5Speed;
        for(int k=0; k<len; k++)
        {
            comparisonArray5.push_back(Eng5[i]);
            j++;
        }
    }
    if(Eng5[0] == 53 || Eng5[0] == 54 || Eng5[0] == 55)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 53 || comparisonArray[0] == 54 || comparisonArray[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 53 || comparisonArray2[0] == 54 || comparisonArray2[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 53 || comparisonArray3[0] == 54 || comparisonArray3[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 53 || comparisonArray4[0] == 54 || comparisonArray4[0] == 55) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    }
    else if(Eng5[0] == 44 || Eng5[0] == 43)
    {

    if(comparisonArray.size() !=0 && ( comparisonArray[0] == 43 || comparisonArray[0] == 44 ))
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray2.size() !=0 && ( comparisonArray2[0] == 43 || comparisonArray2[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray3.size() !=0 && ( comparisonArray3[0] == 43 || comparisonArray3[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    else if(comparisonArray4.size() !=0 && ( comparisonArray4[0] == 43 || comparisonArray4[0] == 44) )
    {
        for (int k=0; k<5; k++)
        {
            comparisonArray5.insert(comparisonArray5.begin(), Eng5[0]);
        }
    }
    }
}

int MainWindow::compare2(int a[], int b[], int c, int d)
{


    if (c >= d )
    {
      for(int i=0; i<d; i++)
      {
          if (a[i] == b[i])
          {
              cout << "Collision detected at track segment" << to_string(a[i]) << endl;
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
                cout << "Collision detected at track segment" << to_string(a[i]) << endl;
                return a[i];
            }
        }
    }

    return -1;
}


int MainWindow::compare3(int a[], int b[], int c[], int a1, int b1, int c1)
{
int i = 0;
    if(a1 >= b1 && b1 >= c1)
    {
        for (; i<c1;i++)
        {
         if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
         {
             cout << "Paths intersect (Code 1). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<b1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect (Code 2). Returning" << endl;
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
             cout << "Paths intersect (Code 3). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<c1;i++)
        {
            if(a[i]==c[i])
            {
                cout << "Paths intersect (Code 4). Returning" << endl;
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
             cout << "Paths intersect (Code 5). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<a1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect (Code 12). Returning" << endl;
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
             cout << "Paths intersect (Code 6). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<c1;i++)
        {
            if(c[i]==b[i])
            {
                cout << "Paths Intersect (Code 7). Returning" << endl;
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
             cout << "Paths intersect (Code 8). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<a1;i++)
        {
            if(a[i]==c[i])
            {
                cout << "Paths intersect (Code 9). Returning" << endl;
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
             cout << "Paths intersect (Code 10). Returning" << endl;
                     if(a[i] == b[i] || a[i] == c[i])
                             return a[i];
                     else
                            return b[i];
         }
        }
        for(;i<b1;i++)
        {
            if(a[i]==b[i])
            {
                cout << "Paths intersect (Code 11). Returning" << endl;
                return a[i];
            }
        }
    }
    return -1;
}


//int MainWindow::compare4(int a[], int b[], int c[], int d[], int a1, int b1, int c1, int d1)
//{
//    int i = 0;
//        if(a1 >= b1 && b1 >= c1 && c1 >= d1)
//        {
//            for (; i<d1;i++)
//            {
//             if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i] || a[i]==d[i] || b[i]==d[i] || c[i] == d[i] )
//             {
//                 cout << "Paths intersect. Returning";
//                         if(a[i] == b[i] || a[i] == c[i] || a[i] == d[i])
//                                 return a[i];
//                         else if (b[i] == c[i] || b[i] == d[i])
//                                return b[i];
//                         else
//                             return c[i];
//             }
//            }
//            for (; i<c1;i++)
//            {
//             if(a[i] == b[i] || a[i]==c[i] || b[i]==c[i])
//             {
//                 cout << "Paths intersect. Returning";
//                         if(a[i] == b[i] || a[i] == c[i])
//                                 return a[i];
//                         else
//                                return b[i];
//             }
//            }
//            for(;i<b1;i++)
//            {
//                if(a[i]==b[i])
//                {
//                    cout << "paths intersect. Returning";
//                    return a[i];
//                }
//            }
//        }
//    return -1;
//}

#endif // CHECKPATHS_H
