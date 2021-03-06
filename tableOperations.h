#ifndef TABLEOPERATIONS_H
#define TABLEOPERATIONS_H
#include <mainwindow.h>
#include <checkPaths.h>
#include <unistd.h>
#include <QString>
#include <ui_mainwindow.h>





void MainWindow::tableOperation(std::list<vertex_t> path, int trainsel, int testInt, int attempt, int islong)
{
testInt++;
    if(testInt >= 4) //if route planning fails, delay the route by 10 seconds and attempt to reroute.
    {
        cout<< "rerouting failed 4 times. Delaying route start time." << endl;
              if(trainsel == 1)
              {
                  int begin = comparisonArray[0];
                  for (int i = 10; i>0; i--)
                  {
                      comparisonArray.insert(comparisonArray.begin(), begin);
                  }
              }
              else if (trainsel == 2)
              {
                  int begin = comparisonArray2[0];
                  for (int i = 10; i>0; i--)
                  {
                      comparisonArray2.insert(comparisonArray2.begin(), begin);
                  }
              }
              else if (trainsel == 3)
              {
                  int begin = comparisonArray3[0];
                  for (int i = 10; i>0; i--)
                  {
                      comparisonArray3.insert(comparisonArray3.begin(), begin);
                  }
              }
              else if (trainsel == 4)
              {
                  int begin = comparisonArray4[0];
                  for (int i = 10; i>0; i--)
                  {
                      comparisonArray4.insert(comparisonArray4.begin(), begin);
                  }
              }
              else if (trainsel == 5)
              {
                  int begin = comparisonArray5[0];
                  for (int i = 10; i>0; i--)
                  {
                      comparisonArray5.insert(comparisonArray5.begin(), begin);
                  }
              }

    return;
    }
    

    int flag = 0;

            // test code for reading the vector to a database
            int pathSize = path.size();
            int numOfRows = pathSize/10; //number of rows on the path table needed
            if (pathSize%10 !=0)
                numOfRows++;
            //cout << "The total number of hops is " << to_string(pathSize) << endl;
            //convert the list to an array
            int *copyarray = new int[pathSize];
            for (int i=0; i<pathSize; i++)
            {
                copyarray[i] = path.front();
                path.pop_front();
            }


            int ArSz = 0; //holds size of the array for comparison later.

            if(trainsel == 1) //if train 1 is selected.
            {
            if(ui->trainselectBox1->currentText() == "Engine 1")
            {
             flag = 1;
             Eng1 = new int[pathSize]; //initialize the internal array
             E1S = pathSize;

             //Determine how long each track segment is, and add this time to the array.
             for(int i=0; i<pathSize; i++)
             {
                 ArSz = ArSz + trackLength(copyarray[i]);
             }

             i1 = ArSz * Eng1Speed; //implement the engine's speed.
             for(int i=0; i<pathSize; i++)
             {
                 Eng1[i] = copyarray[i];
             }
             Eng1Start = Eng1[0];
            }
            }

            else if(trainsel == 2)
            {

            if(ui->trainselectBox2->currentText() == "Engine 2")
            {
                flag = 2;
                Eng2 = new int[pathSize];
                E2S = pathSize;
                for(int i=0; i<pathSize; i++)
                {
                    ArSz = ArSz + trackLength(copyarray[i]);
                }

                i2 = ArSz * Eng2Speed;
                for(int i=0; i<pathSize; i++)
                {
                    Eng2[i] = copyarray[i];
                }
                Eng2Start = Eng2[0];
           }
          
            }
            else if(trainsel == 3)
            {
            
            if(ui->trainselectBox3->currentText() == "Engine 3")
            {
                flag = 3;
                Eng3 = new int[pathSize];
                E3S = pathSize;

                for(int i=0; i<pathSize; i++)
                {
                    ArSz = ArSz + trackLength(copyarray[i]);
                }

                i3 = ArSz * Eng3Speed;
                for(int i=0; i<pathSize; i++)
                {
                    Eng3[i] = copyarray[i];
                }
                Eng3Start = Eng3[0];
            }
            }
            else if(trainsel == 4)
            {
            
            if(ui->trainselectBox4->currentText() == "Engine 4")
            {
                flag = 4;
                Eng4 = new int[pathSize];
                E4S = pathSize;

                for(int i=0; i<pathSize; i++)
                {
                    ArSz = ArSz + trackLength(copyarray[i]);
                }

                i4 = ArSz * Eng4Speed;
                for(int i=0; i<pathSize; i++)
                {
                    Eng4[i] = copyarray[i];
                }
                Eng4Start = Eng4[0];
            }
            }
            else if(trainsel == 5)
            {
            
            if(ui->trainselectBox5->currentText() == "Engine 5")
            {
                flag = 5;
                Eng5 = new int[pathSize];
                E5S = pathSize;

                for(int i=0; i<pathSize; i++)
                {
                    ArSz = ArSz + trackLength(copyarray[i]);
                }

                i5 = ArSz * Eng5Speed;
                for(int i=0; i<pathSize; i++)
                {
                    Eng5[i] = copyarray[i];
                }
                Eng5Start = Eng5[0];
            }
            }
            //check for possible collisions before writing to table
              int check = 0;
           check = checkPaths(attempt, islong);

           if (attempt <= 1) // Only do this on the final itteration of running Dijkstra's
           {
           if(check == -1)
               cout << "no issues detected. Finalizing path" << endl;
           else if (check != -1 && islong == 0) //if the route is a short route, then we attempt to reroute without a delay.
              {

               reroute(trainsel, check);
               //cout << " rerouted!" << endl;
               tableOperation(backupPath, trainsel, testInt, 0, 0);
               return;

               //insert code to reroute or slow down train here. I have the rerouting code partially working, but the throttle controls are currently unavailable, so am leaving this blank for now. The rerouting function is currently unconnected.

              }
           else if (check != -1 && islong == 1) //Upon the failure of a long route, delay and try again
           {
               if(trainsel == 1)
               {
                   int begin = comparisonArray[0];
                   for(int i=5; i>0;i--)
                   {
                       comparisonArray.insert(comparisonArray.begin(), begin);
                   }

               }
               else if (trainsel == 2)
               {
                   int begin = comparisonArray2[0];
                   for(int i=5; i>0;i--)
                   {
                       comparisonArray2.insert(comparisonArray2.begin(), begin);
                   }
               }
               else if (trainsel == 3)
               {
                   int begin = comparisonArray3[0];
                   for(int i=5; i>0;i--)
                   {
                       comparisonArray3.insert(comparisonArray3.begin(), begin);
                   }
               }
               else if (trainsel == 4)
               {
                   int begin = comparisonArray4[0];
                   for(int i=5; i>0;i--)
                   {
                       comparisonArray4.insert(comparisonArray4.begin(), begin);
                   }
               }
               else if (trainsel == 5)
               {
                   int begin = comparisonArray5[0];
                   for(int i=5; i>0;i--)
                   {
                       comparisonArray5.insert(comparisonArray5.begin(), begin);
                   }
               }
           }
           }




            //Use array to fill out traininfo and pathinfo table. Values needed: values in the array, value of the "trainselectBox1" box,
            //final node, initial node, number of rows on the path info table
            QSqlQuery q(ldb);

            q.exec("SELECT COUNT(*) FROM pathinfo;");
            int numRows; //Current number of rows in the path table
            while(q.next())
            {
                numRows = q.value(0).toInt();
            }
            numRows++; //increment this by one to get the first free row.
            int numRows2 = numRows; //duplicate the value for later use
            int itt = 1; //itterator for the loop

            for( int iR = 1; iR<=numOfRows; iR++) //write path data to the sql table
            {
                q.prepare("INSERT INTO pathinfo VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
                q.bindValue(0,numRows); //bind first value to the row on the path table.

                if(numOfRows == (iR))
                    q.bindValue(1, QVariant(QVariant::Int));
                else
                    q.bindValue(1,(numRows+1));

                numRows++;

                if(itt < pathSize){
                q.bindValue(2, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(2, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(3, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(3, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(4, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(4, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(5, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(5, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(6, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(6, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(7, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(7, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(8, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(8, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(9, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(9, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(10, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(10, QVariant(QVariant::String));

                if(itt < pathSize){
                q.bindValue(11, copyarray[itt]);
                itt++;}
                else
                    q.bindValue(11, QVariant(QVariant::String));


                if(!q.exec())
                {
                    cout << "Failed to update database";
                }
             }


        q.prepare("UPDATE traininfo SET current=?,destination=?,next=?,path=? WHERE trainID=?");
        q.bindValue(0, copyarray[0] );
        q.bindValue(1, copyarray[pathSize-1]);
        q.bindValue(2, copyarray[1]);
    //<<<<<<< HEAD
        //q.bindValue(4, numRows2);
        //q.bindValue(3, );
    //=======
        q.bindValue(3, numRows2);

        //QString tmpstr;
        if( trainsel == 1)
        {tmpstr = ui->trainselectBox1->currentText();}
        else if( trainsel == 2)
        {tmpstr = ui->trainselectBox2->currentText();}
        else if( trainsel == 3)
        {tmpstr = ui->trainselectBox3->currentText();}
        else if( trainsel == 4)
        {tmpstr = ui->trainselectBox4->currentText();}
        else if( trainsel == 5)
        {tmpstr = ui->trainselectBox5->currentText();}

        q.bindValue(4, tmpstr);
    //>>>>>>> refs/remotes/train1a/master

        if(!q.exec())
        {
            cout << "Failed to update database";
        }


        //account for a delay
        //
        //
        //




    q.prepare("SELECT * FROM traininfo WHERE trainID=?");
    q.bindValue(0, tmpstr);
    if(!q.exec())
    {
        cout << "Failed to update database";
    }
    QString currentLoc;
    QString nextLoc;
    QString endLoc;
    int nextTab = NULL;
    QString path2;
    QString path3;
    QString path4;
    QString path5;
    QString path6;
    QString path7;
    QString path8;
    QString path9;
    QString path10;
    QString path11;
    int pathTab;
    int currenttab;



    while(q.next())
    {
        currentLoc = q.value(1).toString();
        nextLoc = q.value(2).toString();
        endLoc = q.value(3).toString();
        pathTab = q.value(4).toInt();
    }

    q.prepare("SELECT * FROM pathinfo WHERE pathID=?");
    q.bindValue(0,pathTab);
    if(!q.exec())
    {
        cout << "Failed to update database";
    }
    while(q.next())
    {
        nextTab = q.value(1).toInt();
        path2 = q.value(2).toString();
        path3 = q.value(3).toString();
        path4 = q.value(4).toString();
        path5 = q.value(5).toString();
        path6 = q.value(6).toString();
        path7 = q.value(7).toString();
        path8 = q.value(8).toString();
        path9 = q.value(9).toString();
        path10 = q.value(10).toString();
        path11 = q.value(11).toString();
    }


    int itt2 = 0;

    //The primary output has moved to the timer function. This merely outputs the start location of the active engine.
    if(flag == 1)
    {

        cout << tmpstr.toStdString() << " starting at location " << oursTotheirs(comparisonArray[0]).toStdString() << endl;


    }
    else if (flag == 2)
    {

       
        cout << tmpstr.toStdString()<< " starting at location " << oursTotheirs(comparisonArray2[0]).toStdString() << endl;


    }
    else if (flag == 3)
    {

     
        cout << tmpstr.toStdString()<< " starting at location " << oursTotheirs(comparisonArray3[0]).toStdString() << endl;

    }
    else if (flag == 4)
    {

        
        cout << tmpstr.toStdString()<< " starting at location " << oursTotheirs(comparisonArray4[0]).toStdString() << endl;


    }
    else if (flag == 5)
    {

  
        cout << tmpstr.toStdString()<< " starting at location " << oursTotheirs(comparisonArray5[0]).toStdString() << endl;

        }

    










//    //for(int i=0; i<3; i++)
//    while( itt2 == 0)
//    {
//        int tLoop = 0;

//        //checkSwitches(currentLoc, nextLoc);
//        //cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << endLoc.toStdString() << endl;
//        //currentLoc = nextLoc;

//    if(path2 != NULL){

//        //for(;tLoop)

//        nextLoc = path2;
//        checkSwitches(currentLoc, path2);

//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path2.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//    }

//    if(path3 != NULL){
//        nextLoc = path3;
//        checkSwitches(currentLoc, path3);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path3.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//    }

//     if(path4 != NULL){
//        nextLoc = path4;
//        checkSwitches(currentLoc, path4);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path4.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//     }

//      if(path5 != NULL){
//        nextLoc = path5;
//        checkSwitches(currentLoc, path5);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path5.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path6 != NULL){
//        nextLoc = path6;
//        checkSwitches(currentLoc, path6);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path6.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path7 != NULL){
//        nextLoc = path7;
//        checkSwitches(currentLoc, path7);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path7.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path8 != NULL){
//        nextLoc = path8;
//        checkSwitches(currentLoc, path8);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path8.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path9 != NULL){
//        nextLoc = path9;
//        checkSwitches(currentLoc, path9);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path9.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path10 != NULL){
//        nextLoc = path10;
//        checkSwitches(currentLoc, path10);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path10.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//      }

//      if(path11 != NULL){
//        nextLoc = path11;
//        checkSwitches(currentLoc, path11);
//        cout << tmpstr.toStdString() << " Moved from " << currentLoc.toStdString() << " to " << path11.toStdString() << endl;
//        currentLoc = nextLoc;
//        //call to check for occupied track sections ahead and delay
//    }

      if(nextTab != NULL){

          q.prepare("SELECT * FROM pathinfo WHERE pathID=?");
          q.bindValue(0,nextTab);
          if(!q.exec())
          {
              cout << "Failed to update database 123";
          }

          //int nextTab2 = nextTab;

          while(q.next()) //continue reading from SQL table.
          {
              currenttab = q.value(0).toInt();
              nextTab = q.value(1).toInt();
              path2 = q.value(2).toString();
              path3 = q.value(3).toString();
              path4 = q.value(4).toString();
              path5 = q.value(5).toString();
              path6 = q.value(6).toString();
              path7 = q.value(7).toString();
              path8 = q.value(8).toString();
              path9 = q.value(9).toString();
              path10 = q.value(10).toString();
              path11 = q.value(11).toString();
          //}






          q.prepare("UPDATE traininfo SET current=?,next=?,path=? WHERE trainID =?");
          q.bindValue(0,currentLoc);
          q.bindValue(1,path2);
          q.bindValue(2,currenttab);
          q.bindValue(3,tmpstr);
          if(!q.exec())
          {
              cout << "Failed to update database 456";
          }

    }
      }
      else
         itt2++;


}




#endif // TABLEOPERATIONS_H
