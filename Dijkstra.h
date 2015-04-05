#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <climits>
#include <QDebug>
#include <mainwindow.h>
//#include <dijkstraswitchcheck.h>
long dijcheckSwitches(long dcN, long dnN);
int forbiddenTurnU(int u, int v);
int forbiddenTurnV(int u, int v);
using namespace std;
typedef long vertex_t;
typedef long weight_t;

const weight_t max_weight = LONG_MAX;

struct neighbor
{
    vertex_t target;
    weight_t weight;
    neighbor(vertex_t arg_target, weight_t arg_weight): target(arg_target), weight(arg_weight) { }
};
typedef std::vector<std::vector<neighbor> > adjacency_list_t;

void DijkstraComputePaths(vertex_t source,const adjacency_list_t &adjacency_list, std::vector<weight_t> &min_distance,std::vector<vertex_t> &previous)
{
    //int forbidden1, forbidden2 = 0;
    //int oldestU, oldU, recentU = -1;
    //int oldererSw, oldestSw, oldSw, lastSw, currentSw = 0;
    //int sw3, sw2, sw1 = -1;
    //long switchNumber = -1;
    int u1, u2, u3, u4, u5, u6, u7, u8 = -1;
    int v1, v2, v3, v4, v5, v6, v7, v8 = -1;
    int U, V;
    int n = adjacency_list.size();
    min_distance.clear();
    min_distance.resize(n, max_weight);
    min_distance[source] = 0;
    previous.clear();
    previous.resize(n, -1);
    std::set<std::pair<weight_t, vertex_t> > vertex_queue;
        vertex_queue.insert(std::make_pair(min_distance[source], source));

    while (!vertex_queue.empty())
    {
        weight_t dist = vertex_queue.begin()->first;
        vertex_t u = vertex_queue.begin()->second;
        vertex_queue.erase(vertex_queue.begin());

        // Visit each edge exiting u
        const std::vector<neighbor> &neighbors = adjacency_list[u];
        for (std::vector<neighbor>::const_iterator neighbor_iter = neighbors.begin();
        neighbor_iter != neighbors.end();
        neighbor_iter++)
        {
            vertex_t v = neighbor_iter->target;
            weight_t weight = neighbor_iter->weight;
            weight_t distance_through_u = dist + weight;

             if (distance_through_u < min_distance[v])
            {
                 //qDebug() << "input " << u << v << endl;

                 U = forbiddenTurnU(u, v);
                 V = forbiddenTurnV(u ,v);
                 //qDebug() << "generated " << U << V << endl;
                if (U != -1){
                     u8 = u7;
                     u7 = u6;
                     u6 = u5;
                     u5 = u4;
                     u4 = u3;
                     u3 = u2;
                     u2 = U;
                     u1 = U;
                 }
                 if (V != -1){
                     v8 = v7;
                     v7 = v6;
                     v6 = v5;
                     v5 = v4;
                     v4 = v3;
                     v3 = v2;
                     v2 = V;
                     v1 = V;
                 }

                 //qDebug() << u5 << v5 << "," << u4 << v4 << "," << u3 << v3 << "," << u2 << v2 << "," << u1 << v1 << endl;
                 if ((u == u1 && v == v1) ||(u == u2 && v == v2) || (u == u3 && v == v3) ||
                         (u == u4 && v == v4) || (u == u5 && v == v5)){
                     //qDebug() << "skipped" << endl;
                 }
                 else{
                //qDebug() << "s" << u << v << endl;
                vertex_queue.erase(std::make_pair(min_distance[v], v));
                min_distance[v] = distance_through_u;
                previous[v] = u;
                //qDebug() << "e" << u << v << endl;
                vertex_queue.insert(std::make_pair(min_distance[v], v));
}
            }

        }
    }

}

std::list<vertex_t> DijkstraGetShortestPathTo(vertex_t vertex, const std::vector<vertex_t> &previous)
{

    std::list<vertex_t> path;
    for (; vertex != -1; vertex = previous[vertex])
        path.push_front(vertex);
    return path;
}

int forbiddenTurnU(int u, int v)
{
    if ( u == 75 && v == 42)
        return 42;
    else if ( u == 76 && v == 42)
        return 42;
    else if ( u == 42 && v == 41)
        return 41;
    else if ( u == 46 && v == 41)
        return 41;
    else if ( u == 41 && v == 40)
        return 40;
    else if ( u == 45 && v == 40)
        return 40;
    else if ( u == 15 && v == 14)
        return 14;
    else if ( u == 16 && v == 14)
        return 14;
    else if ( u == 8 && v == 7)
        return 7;
    else if ( u == 9 && v == 7)
        return 7;
    else if ( u == 5 && v == 4)
        return 4;
    else if ( u == 6 && v == 4)
        return 4;
    else if ( u == 11 && v == 10)
        return 10;
    else if ( u == 12 && v == 10)
        return 10;
    else if ( u == 14 && v == 16)
        return 16;
    else if ( u == 12 && v == 16)
        return 16;
    else if ( u == 63 && v == 62)
        return 62;
    else if ( u == 64 && v == 62)
        return 62;
    else if ( u == 18 && v == 17)
        return 17;
    else if ( u == 19 && v == 17)
        return 17;
    else if ( u == 3 && v == 5)
        return 5;
    else if ( u == 4 && v == 5)
        return 5;
    else if ( u == 21 && v == 20)
        return 20;
    else if ( u == 22 && v == 20)
        return 20;
    else if ( u == 23 && v == 22)
        return 22;
    else if ( u == 24 && v == 22)
        return 22;
    else if ( u == 54 && v == 52)
        return 52;
    else if ( u == 53 && v == 52)
        return 52;
    else if ( u == 52 && v == 51)
        return 51;
    else if ( u == 55 && v == 51)
        return 51;
    else if ( u == 51 && v == 26)
        return 26;
    else if ( u == 25 && v == 26)
        return 26;
    else if ( u == 27 && v == 28)
        return 28;
    else if ( u == 26 && v == 28)
        return 28;
    else if ( u == 21 && v == 29)
        return 29;
    else if ( u == 28 && v == 29)
        return 29;
    else if ( u == 0 && v == 2)
        return 2;
    else if ( u == 71 && v == 2)
        return 2;
    else if ( u == 1 && v == 0)
        return 0;
    else if ( u == 2 && v == 0)
        return 0;
    else if ( u == 47 && v == 49)
        return 49;
    else if ( u == 48 && v == 49)
        return 49;
    else if ( u == 30 && v == 31)
        return 31;
    else if ( u == 70 && v == 31)
        return 31;
    else if ( u == 66 && v == 65)
        return 65;
    else if ( u == 67 && v == 65)
        return 65;
    else if ( u == 37 && v == 39)
        return 39;
    else if ( u == 38 && v == 39)
        return 39;
    else if ( u == 68 && v == 36)
        return 36;
    else if ( u == 35 && v == 36)
        return 36;
    else if ( u == 33 && v == 32)
        return 32;
    else if ( u == 34 && v == 32)
        return 32;
    else if ( u == 32 && v == 34)
        return 34;
    else if ( u == 50 && v == 34)
        return 34;

    else
        return -1;
}

int forbiddenTurnV(int u, int v)
{
    if ( u == 75 && v == 42)
        return 76;
    else if ( u == 76 && v == 42)
        return 75;
    else if ( u == 42 && v == 41)
        return 46;
    else if ( u == 46 && v == 41)
        return 42;
    else if ( u == 41 && v == 40)
        return 45;
    else if ( u == 45 && v == 40)
        return 41;
    else if ( u == 15 && v == 14)
        return 16;
    else if ( u == 16 && v == 14)
        return 15;
    else if ( u == 8 && v == 7)
        return 9;
    else if ( u == 9 && v == 7)
        return 8;
    else if ( u == 5 && v == 4)
        return 6;
    else if ( u == 6 && v == 4)
        return 5;
    else if ( u == 11 && v == 10)
        return 12;
    else if ( u == 12 && v == 10)
        return 11;
    else if ( u == 14 && v == 16)
        return 12;
    else if ( u == 12 && v == 16)
        return 14;
    else if ( u == 63 && v == 62)
        return 64;
    else if ( u == 64 && v == 62)
        return 63;
    else if ( u == 18 && v == 17)
        return 19;
    else if ( u == 19 && v == 17)
        return 18;
    else if ( u == 3 && v == 5)
        return 4;
    else if ( u == 4 && v == 5)
        return 3;
    else if ( u == 21 && v == 20)
        return 22;
    else if ( u == 22 && v == 20)
        return 21;
    else if ( u == 23 && v == 22)
        return 24;
    else if ( u == 24 && v == 22)
        return 23;
    else if ( u == 54 && v == 52)
        return 53;
    else if ( u == 53 && v == 52)
        return 54;
    else if ( u == 52 && v == 51)
        return 55;
    else if ( u == 55 && v == 51)
        return 52;
    else if ( u == 51 && v == 26)
        return 25;
    else if ( u == 25 && v == 26)
        return 51;
    else if ( u == 27 && v == 28)
        return 26;
    else if ( u == 26 && v == 28)
        return 27;
    else if ( u == 21 && v == 29)
        return 28;
    else if ( u == 28 && v == 29)
        return 84;
    else if ( u == 0 && v == 2)
        return 71;
    else if ( u == 71 && v == 2)
        return 0;
    else if ( u == 1 && v == 0)
        return 2;
    else if ( u == 2 && v == 0)
        return 1;
    else if ( u == 47 && v == 49)
        return 48;
    else if ( u == 48 && v == 49)
        return 47;
    else if ( u == 30 && v == 31)
        return 70;
    else if ( u == 70 && v == 31)
        return 30;
    else if ( u == 66 && v == 65)
        return 67;
    else if ( u == 67 && v == 65)
        return 66;
    else if ( u == 37 && v == 39)
        return 38;
    else if ( u == 38 && v == 39)
        return 37;
    else if ( u == 68 && v == 36)
        return 35;
    else if ( u == 35 && v == 36)
        return 68;
    else if ( u == 33 && v == 32)
        return 34;
    else if ( u == 34 && v == 32)
        return 33;
    else if ( u == 32 && v == 34)
        return 50;
    else if ( u == 50 && v == 34)
        return 32;

    else
        return -1;
}

long dijcheckSwitches(long dcN, long dnN)
{
    int switchUsed = -1;
    if (dcN == 36)
    {
        if(dnN == 35)
            switchUsed = 11;
        else if(dnN == 68)
           switchUsed = 11;


           return switchUsed;
    }
    else if (dcN == 39)
    {
        if(dnN == 38)
            switchUsed = 21;
        else if(dnN == 37)
           switchUsed = 21;

           return switchUsed;
    }

    else if (dcN == 65)
    {
        if(dnN == 66)
            switchUsed = 43;
        else if(dnN == 67)
           switchUsed = 43;
           return switchUsed;
    }
    else if (dcN == 31)
    {
        if(dnN == 70)
            switchUsed = 42;
        else if(dnN == 30)
           switchUsed = 42;
           return switchUsed;
    }
    else if (dcN == 32)
    {
        if(dnN == 33)
            switchUsed = 13;
        else if(dnN == 34)
           switchUsed = 13;
           return switchUsed;
    }
    else if (dcN == 34)
    {
        if(dnN == 50)
            switchUsed = 12;
        else if(dnN == 32)
            switchUsed = 12;
           return switchUsed;
    }
    else if (dcN == 0)
    {
        if(dnN == 1)
            switchUsed = 22;
        else if(dnN == 2)
            switchUsed = 22;
           return switchUsed;
    }
    else if (dcN == 2)
    {
        if(dnN == 71)
            switchUsed = 23;
        else if(dnN == 0)
            switchUsed = 23;
           return switchUsed;
    }
    else if (dcN == 29)
    {
        if(dnN == 21)
            switchUsed = 24;
        else if(dnN == 28)
            switchUsed = 24;
           return switchUsed;
    }
    else if (dcN == 28)
    {
        if(dnN == 26)
            switchUsed = 34;
        else if(dnN == 27)
           switchUsed = 34;
        else if(dnN == 29)
           switchUsed = 24;
           return switchUsed;
    }
    else if (dcN == 26)
    {
        if(dnN == 51)
            switchUsed = 33;
        else if(dnN == 25)
           switchUsed = 33;
        else if(dnN == 28)
            switchUsed = 34;
           return switchUsed;
    }
    else if (dcN == 51)
    {
        if(dnN == 52)
           switchUsed = 32;
        else if(dnN == 55)
           switchUsed = 32;
        else if(dnN == 26)
            switchUsed = 33;
           return switchUsed;
    }
    else if (dcN == 52)
    {
        if(dnN == 53)
            switchUsed = 31;
        else if(dnN == 54)
           switchUsed = 31;
        else if(dnN == 51)
            switchUsed = 32;
           return switchUsed;
    }
    else if (dcN == 49)
    {
        if(dnN == 47)
            switchUsed = 41;
        else if(dnN == 48)
           switchUsed = 41;
           return switchUsed;
    }
    else if (dcN == 56)
    {
        if(dnN == 57)
           switchUsed = 14;
        else if(dnN == 58)
           switchUsed = 14;
           return switchUsed;
    }
    else if (dcN == 42)
    {
        if(dnN == 43)
            switchUsed = 63;
        else if(dnN == 44)
           switchUsed = 63;
        else if(dnN == 41)
           switchUsed = 62;
           return switchUsed;
    }
    else if (dcN == 41)
    {
        if(dnN == 42)
            switchUsed = 62;
        else if(dnN == 46)
           switchUsed = 62;
        else if(dnN == 40)
            switchUsed = 61;
           return switchUsed;
    }
    else if (dcN == 40)
    {
        if(dnN == 41)
            switchUsed = 61;
        else if(dnN == 45)
           switchUsed = 61;
           return switchUsed;
    }
    else if (dcN == 61)
    {
        if(dnN == 59)
            switchUsed = 84;
        else if(dnN == 60)
           switchUsed = 84;
           return switchUsed;
    }
    else if (dcN == 22)
    {
        if(dnN == 24)
            switchUsed = 64;
        else if(dnN == 23)
           switchUsed = 64;
        else if(dnN == 20)
            switchUsed = 75;
           return switchUsed;
    }
    else if (dcN == 20)
    {
        if(dnN == 21)
            switchUsed = 74;
        else if(dnN == 22)
           switchUsed = 74;
           return switchUsed;
    }
    else if (dcN == 4)
    {
        if(dnN == 6)
            switchUsed = 73;
        else if(dnN == 5)
           switchUsed = 73;
           return switchUsed;
    }
    else if (dcN == 5)
    {
        if(dnN == 3)
            switchUsed = 72;
        else if(dnN == 4)
           switchUsed = 72;
           return switchUsed;
    }
    else if (dcN == 7)
    {
        if(dnN == 9)
            switchUsed = 71;
        else if(dnN == 8)
           switchUsed = 71;
           return switchUsed;
    }
    else if (dcN == 62)
    {
        if(dnN == 63)
            switchUsed = 52;
        else if(dnN == 64)
           switchUsed = 52;
           return switchUsed;
    }
    else if (dcN == 17)
    {
        if(dnN == 19)
            switchUsed = 51;
        else if(dnN == 18)
           switchUsed = 51;
           return switchUsed;
    }
    else if (dcN == 16)
    {
        if(dnN == 12)
            switchUsed = 83;
        else if(dnN == 14)
           switchUsed = 83;
           return switchUsed;
    }
    else if (dcN == 14)
    {
        if(dnN == 15)
            switchUsed = 82;

        else if(dnN == 16)
           switchUsed = 82;
           return switchUsed;
    }
    else if (dcN == 10)
    {
        if(dnN == 12)
            switchUsed = 81;
        else if(dnN == 11)
            switchUsed = 81;
            return switchUsed;
    }
    else if (dcN == 35 && dnN == 36)
        switchUsed = 11;
    else if (dcN == 38 && dnN == 39)
        switchUsed = 21;
    else if (dcN == 66 && dnN == 65)
        switchUsed = 43;
    else if (dcN == 70 && dnN == 31)
        switchUsed = 42;
    else if (dcN == 33 && dnN == 32)
        switchUsed = 13;
    else if (dcN == 50 && dnN == 34)
        switchUsed = 12;
    else if (dcN == 1 && dnN == 0)
        switchUsed = 22;
    else if (dcN == 71 && dnN == 2)
        switchUsed = 23;
    else if (dcN == 53 && dnN == 52)
        switchUsed = 31;
    else if (dcN == 47 && dnN == 49)
        switchUsed = 41;
    else if (dcN == 57 && dnN == 56)
        switchUsed = 14;
    else if (dcN == 43 && dnN == 42)
        switchUsed = 63;
    else if (dcN == 59 && dnN == 61)
        switchUsed = 84;
    else if (dcN == 24 && dnN == 22)
        switchUsed = 64;
    else if (dcN == 6 && dnN == 4)
        switchUsed = 73;
    else if (dcN == 3 && dnN == 5)
        switchUsed = 72;
    else if (dcN == 9 && dnN == 7)
        switchUsed = 71;
    else if (dcN == 63 && dnN == 62)
        switchUsed = 52;
    else if (dcN == 19 && dnN == 17)
        switchUsed = 51;
    else if (dcN == 15 && dnN == 14)
        switchUsed = 82;


    else if(dcN == 21)
    {
        if (dnN == 20)
            switchUsed = 74;
        else if (dnN == 29)
            switchUsed = 24;

        return switchUsed;
    }
    else if(dcN == 12)
    {
        if (dnN == 16)
            switchUsed = 83;
        else if (dnN == 10)
            switchUsed = 81;

        return switchUsed;
    }
    else if (dcN == 68 && dnN == 36)
        switchUsed = 11;
    else if (dcN == 37 && dnN == 39)
        switchUsed = 21;
    else if (dcN == 67 && dnN == 65)
        switchUsed = 43;
    else if (dcN == 30 && dnN == 31)
        switchUsed = 42;
    else if (dcN == 27 && dnN == 28)
        switchUsed = 34;
    else if (dcN == 25 && dnN == 26)
        switchUsed = 33;
    else if (dcN == 55 && dnN == 51)
        switchUsed = 32;
    else if (dcN == 54 && dnN == 52)
        switchUsed = 31;
    else if (dcN == 48 && dnN == 49)
        switchUsed = 41;
    else if (dcN == 58 && dnN == 56)
        switchUsed = 14;
    else if (dcN == 44 && dnN == 42)
        switchUsed = 63;

    else if (dcN == 46 && dnN == 41)
        switchUsed = 62;
    else if (dcN == 45 && dnN == 40)
        switchUsed = 61;
    else if (dcN == 60 && dnN == 61)
        switchUsed = 84;
    else if (dcN == 23 && dnN == 22)
        switchUsed = 64;
    else if (dcN == 8 && dnN == 7)
        switchUsed = 71;
    else if (dcN == 64 && dnN == 62)
        switchUsed = 52;
    else if (dcN == 18 && dnN == 17)
        switchUsed = 51;
    else if (dcN == 11 && dnN == 10)
        switchUsed = 81;
    else
        switchUsed = -1;

        return switchUsed;
}

QString MainWindow::oursTotheirs(int number)
{
    if (number == 43)
        return "U-3";
    if (number == 75)
        return "4-9";
    if (number == 42)
        return "42";
    if (number == 41)
        return "41";
    if (number == 40)
        return "5-13";
    if (number == 45)
        return "5-14";
    if (number == 78)
        return "5-15";
    if (number == 47)
        return "5-16";
    if (number == 46)
        return "5-1";
    if (number == 77)
        return "4-11";
    if (number == 48)
        return "4-10";
    if (number == 49)
        return "U-1";
    if (number == 44)
        return "U-2";
    if (number == 76)
        return "4-12";
    if (number == 95)
        return "2-9";
    if (number == 94)
        return "2-8";
    if (number == 57)
        return "2-7";
    if (number == 58)
        return "2-6";
    if (number == 56)
        return "2-5";
    if (number == 59)
        return "2-11";
    if (number == 60)
        return "2-10";
    if (number == 61)
        return "2-12";
    if (number == 93)
            return "2-13";
    if (number == 15)
            return "5-4";
    if (number == 16)
            return "U-8";
    if (number == 17)
            return "U-8";
    if (number == 14)
            return "U-9";
    if (number == 82)
            return "U-10";
    if (number == 79)
            return "U-12";
    if (number == 13)
            return "U-11";
    if (number == 12)
            return "U-13";
    if (number == 11)
            return "U-14";
    if (number == 10)
            return "U-16";
    if (number == 98)
            return "U-17";
    if (number == 80)
            return "U-15";
    if (number == 8)
            return "U-18";
    if (number == 9)
            return "U-19";
    if (number == 7)
            return "U-20";
    if (number == 5)
            return "U-21";
    if (number == 6)
            return "1-4R";
    if (number == 4)
            return "U-23";
    if (number == 3)
            return "U-22";
    if (number == 81)
            return "U-31";
    if (number == 1)
            return "U-33";
    if (number == 0)
            return "U-44";
    if (number == 2)
            return "U-32";
    if (number == 84)
            return "84";
    if (number == 83)
            return "83";
    if (number == 21)
            return "21";
    if (number == 20)
            return "U-24";
    if (number == 18)
            return "U-24";
    if (number == 19)
            return "1-4R";
    if (number == 62)
            return "2-14";
    if (number == 63)
            return "2-15";
    if (number == 92)
            return "1-2";
    if (number == 64)
            return "2-16";
    if (number == 22)
            return "22";
    if (number == 24)
            return "24";
    if (number == 23)
            return "23";
    if (number == 25)
            return "25";
    if (number == 27)
            return "27";
    if (number == 26)
            return "26";
    if (number == 51)
            return "51";
    if (number == 52)
            return "52";
    if (number == 85)
            return "U-40";
    if (number == 87)
            return "U-42";
    if (number == 86)
            return "U-41";
    if (number == 53)
            return "U-30";
    if (number == 54)
            return "U-29";
    if (number == 55)
            return "U-28";
    if (number == 28)
            return "28";
    if (number == 29)
            return "U-62";
    if (number == 30)
            return "U-62";
    if (number == 70)
            return "1-3R";
    if (number == 71)
            return "1-3R";
    if (number == 39)
            return "U-45";
    if (number == 37)
            return "U-47";
    if (number == 38)
            return "U-46";
    if (number == 66)
            return "2-2";
    if (number == 65)
            return "2-3";
    if (number == 67)
            return "2-1";
    if (number == 97)
            return "1-1";
    if (number == 96)
            return "2-4";
    if (number == 32)
            return "U-59";
    if (number == 31)
            return "U-59";
    if (number == 50)
            return "U-61";
    if (number == 34)
            return "U-57";
    if (number == 33)
            return "U-58";
    if (number == 89)
            return "U-53";
    if (number == 90)
            return "U-55";
    if (number == 69)
            return "U-56";
    if (number == 68)
            return "U-54";
    if (number == 35)
            return "U-52";
    if (number == 36)
            return "U-51";
    if (number == 91)
            return "U-50";
    if (number == 88)
            return "U-48";
    else
        return "-1";
}

int MainWindow::theirsToours(QString string)
{
    if (string == "U-3")
        return 43;
    if (string == "4-9")
        return 75;
    if (string == "42")
        return 42;
    if (string == "41")
        return 41;
    if (string == "5-13")
        return 40;
    if (string == "5-14")
        return 45;
    if (string == "5-15")
        return 78;
    if (string == "5-16")
        return 47;
    if (string == "5-1")
        return 46;
    if (string == "4-11")
        return 77;
    if (string == "4-10")
        return 48;
    if (string == "U-1")
        return 49;
    if (string == "U-2")
        return 44;
    if (string == "4-12")
        return 76;
    if (string == "2-9")
        return 95;
    if (string == "2-8")
        return 94;
    if (string == "2-7")
        return 57;
    if (string == "2-6")
        return 58;
    if (string == "2-5")
        return 56;
    if (string == "2-11")
        return 59;
    if (string == "2-10")
        return 60;
    if (string == "2-12")
        return 61;
    if (string == "2-13")
            return 93;
    if (string == "5-4")
            return 15;
    if (string == "U-8")
            return 16;
    if (string == "U-8")
            return 17;
    if (string == "U-9")
            return 14;
    if (string == "U-10")
            return 82;
    if (string == "U-12")
            return 79;
    if (string == "U-11")
            return 13;
    if (string == "U-13")
            return 12;
    if (string == "U-14")
            return 11;
    if (string == "U-16")
            return 10;
    if (string == "U-17")
            return 98;
    if (string == "U-15")
            return 80;
    if (string == "U-18")
            return 8;
    if (string == "U-19")
            return 9;
    if (string == "U-20")
            return 7;
    if (string == "U-21")
            return 5;
    if (string == "1-4R")
            return 6;
    if (string == "U-23")
            return 4;
    if (string == "U-22")
            return 3;
    if (string == "U-31")
            return 81;
    if (string == "U-33")
            return 1;
    if (string == "U-44")
            return 0;
    if (string == "U-32")
            return 2;
    if (string == "84")
            return 84;
    if (string == "83")
            return 83;
    if (string == "21")
            return 21;
    if (string == "U-24")
            return 20;
    if (string == "U-24")
            return 18;
    if (string == "1-4R")
            return 19;
    if (string == "2-14")
            return 62;
    if (string == "2-15")
            return 63;
    if (string == "1-2")
            return 92;
    if (string == "2-16")
            return 64;
    if (string == "22")
            return 22;
    if (string == "24")
            return 24;
    if (string == "23")
            return 23;
    if (string == "25")
            return 25;
    if (string == "27")
            return 27;
    if (string == "26")
            return 26;
    if (string == "51")
            return 51;
    if (string == "52")
            return 52;
    if (string == "U-40")
            return 85;
    if (string == "U-42")
            return 87;
    if (string == "U-41")
            return 86;
    if (string == "U-30")
            return 53;
    if (string == "U-29")
            return 54;
    if (string == "U-28")
            return 55;
    if (string == "28")
            return 28;
    if (string == "U-62")
            return 29;
    if (string == "U-62")
            return 30;
    if (string == "1-3R")
            return 70;
    if (string == "1-3R")
            return 71;
    if (string == "U-45")
            return 39;
    if (string == "U-47")
            return 37;
    if (string == "U-46")
            return 38;
    if (string == "2-2")
            return 66;
    if (string == "2-3")
            return 65;
    if (string == "2-1")
            return 67;
    if (string == "1-1")
            return 97;
    if (string == "2-4")
            return 96;
    if (string == "U-59")
            return 32;
    if (string == "U-59")
            return 31;
    if (string == "U-61")
            return 50;
    if (string == "U-57")
            return 34;
    if (string == "U-58")
            return 33;
    if (string == "U-53")
            return 89;
    if (string == "U-55")
            return 90;
    if (string == "U-56")
            return 69;
    if (string == "U-54")
            return 68;
    if (string == "U-52")
            return 35;
    if (string == "U-51")
            return 36;
    if (string == "U-50")
            return 91;
    if (string == "U-48")
            return 88;
    else
        return -1;
}

#endif // DIJKSTRA_H
