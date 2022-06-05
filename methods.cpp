#include <iostream>
#include <math.h>
#include <list>
#include <iterator>
#include <stdexcept>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool is_segment_regular(pair<int, int> point_1, pair<int, int> point_2, int metric = 4) {
    auto x_1 = point_1.first;
    auto y_1 = point_1.second;
    auto x_2 = point_2.first;
    auto y_2 = point_2.second;

    if (metric == 2) {
        if ((x_1 == x_2) && (y_1 == y_2)) return true;
        if ((x_1 != x_2) && (y_1 != y_2)) return false;
        return true;
    }
    else if (metric == 4) {
        if ((x_1 == x_2) && (y_1 == y_2)) return true;
        if ((x_1 != x_2) && (y_1 != y_2) && (abs(x_1 - x_2) != abs(y_1 - y_2))) return false;
        return true;
    }
    else {
        throw std::invalid_argument("wrong metric!");
    }
}

int direction(pair<int, int> point_1, pair<int, int> point_2, int metric = 4) {
    if (!is_segment_regular(point_1, point_2, metric)) {
        throw std::invalid_argument("is not regular");
    }

    auto x_1 = point_1.first;
    auto y_1 = point_1.second;
    auto x_2 = point_2.first;
    auto y_2 = point_2.second;

    if (metric == 2) {
        if ((x_1 == x_2) && (y_1 == y_2)) return 0;
        else if ((x_1 == x_2) && (y_1 < y_2)) return 1;
        else if ((x_1 == x_2) && (y_1 > y_2)) return 3;
        else if ((x_1 < x_2) && (y_1 == y_2)) return 2;
        else if ((x_1 > x_2) && (y_1 == y_2)) return 4;
    }
    if (metric == 4) {
        if ((x_1 == x_2) && (y_1 == y_2)) return 0;
        else if ((x_1 == x_2) && (y_1 < y_2)) return 1;
        else if ((x_1 == x_2) && (y_1 > y_2)) return 5;
        else if ((x_1 < x_2) && (y_1 < y_2)) return 2;
        else if ((x_1 > x_2) && (y_1 > y_2)) return 6;
        else if ((x_1 < x_2) && (y_1 == y_2)) return 3;
        else if ((x_1 > x_2) && (y_1 == y_2)) return 7;
        else if ((x_1 < x_2) && (y_1 > y_2)) return 4;
        else if ((x_1 > x_2) && (y_1 < y_2)) return 8;
    }
    else {
        throw std::invalid_argument("wrong metric!");
    }
}

int dist_square(pair<int, int> point_1, pair<int, int> point_2) {
    auto x_1 = point_1.first;
    auto y_1 = point_1.second;
    auto x_2 = point_2.first;
    auto y_2 = point_2.second;

    return pow((x_2 - x_1), 2) + pow((y_2 - y_1), 2);
}

pair<int, int> middle_HS_point(pair<int, int> point_1, pair<int, int> point_2, int metric = 4) {
    auto x_1 = point_1.first;
    auto y_1 = point_1.second;
    auto x_2 = point_2.first;
    auto y_2 = point_2.second;

    if (metric == 2) {
        return make_pair(x_1, y_2);
    }
    if (metric == 4) {
        int x_m, y_m;
        if (abs(x_2 - x_1) >= abs(y_2 - y_1)) {
            if (x_2 >= x_1) {
                x_m = x_1 + abs(y_2 - y_1);
                y_m = y_2;
            }
            else {
                x_m = x_1 - abs(y_2 - y_1);
                y_m = y_2;
            }
        }
        else {
            if (y_2 >= y_1) {
                y_m = y_1 + abs(x_2 - x_1);
                x_m = x_2;
            }
            else {
                y_m = y_1 - abs(x_2 - x_1);
                x_m = x_2;
            }
        }
        return make_pair(x_m, y_m);
    }
    else {
        throw std::invalid_argument("wrong metric!");
    }
}

class trajectory {
private:
    list<pair<int, int>> traj;
    int metric;

public:

    trajectory(int _metric = 4) : metric(_metric) {}

    trajectory(list<pair<int, int>> _list, int _metric = 4) : traj(_list), metric(_metric) {}

    void insert(pair<int, int> point) {
        traj.insert(traj.end(), point);
    }
    void erase(list<pair<int, int>>::iterator iter) {
        traj.erase(iter);
    }
    void reverse() {
        traj.reverse();
    }

    list<pair<int, int>>::iterator begin() {
        return traj.begin();
    }
    list<pair<int, int>>::iterator end() {
        return traj.end();
    }
    list<pair<int, int>>::reverse_iterator rbegin() {
        return traj.rbegin();
    }
    list<pair<int, int>>::reverse_iterator rend() {
        return traj.rend();
    }

    int metr() {
        return metric;
    }
    int size() {
        return traj.size();
    }

    bool is_regular() {
        if (traj.size() <= 1) return true;
        auto start = traj.begin();
        auto last = traj.end();
        --last;
        for (auto iter = start; iter != last; ++iter) {
            auto point_1 = *iter;
            auto iter_next = iter;
            ++iter_next;
            auto point_2 = *iter_next;
            if (!is_segment_regular(point_1, point_2, metric)) {
                return false;
            }
        }
        return true;
    }

    void clean() {
        if (traj.size() <= 1) return;
        auto iter = traj.begin();
        auto last = traj.end();
        --last;
        while (iter != last) {

            auto point_1 = *iter;
            auto iter_next = iter;
            ++iter_next;
            auto point_2 = *iter_next;

            if (direction(point_1, point_2, metric) == 0) {
                iter = traj.erase(iter);
                --iter;
                continue;
            }
            else {
                ++iter_next;
                if (iter_next == traj.end()) return;
                auto point_3 = *iter_next;

                if (direction(point_2, point_3, metric) == 0) {
                    ++iter;
                    continue;
                }
                else {
                    if (direction(point_1, point_2, metric) % metric
                        == direction(point_2, point_3, metric) % metric) {
                        ++iter;
                        iter = traj.erase(iter);
                        --iter;
                    }
                    else {
                        ++iter;
                    }
                }
            }
        }
    }

    bool is_active_HS(list<pair<int, int>>::iterator iter, int order) {
        pair<int, int> point_1, point_2, point_3;
        if (order == 1) {
            point_1 = *iter;
            ++iter;
            if (iter == traj.end()) return false;
            point_2 = *iter;
            ++iter;
            if (iter == traj.end()) return false;
            point_3 = *iter;
        }
        else if (order == -1) {
            point_3 = *iter;
            if (iter == traj.begin()) return false;
            --iter;
            point_2 = *iter;
            if (iter == traj.begin()) return false;
            --iter;
            point_1 = *iter;
        }
        auto d_1_2 = direction(point_1, point_2, metric);
        auto d_2_3 = direction(point_2, point_3, metric);

        if (((2 * metric) + d_2_3 - d_1_2) % (2 * metric) == 1) return true;
        else return false;
    }

    void print(string file_name) {
        ofstream cout(file_name);
        for (auto iter = traj.begin(); iter != traj.end(); ++iter) {
            auto point = *iter;
            cout << point.first << ", " << point.second << endl;
        }
    }


};

class triangle {
public:
    trajectory side_1, side_2, side_3;
    trajectory tail_1, tail_2, tail_3;
    int metric;

    triangle() {}

    triangle(trajectory traj_1, trajectory traj_2, trajectory traj_3) :
        side_1(traj_1), side_2(traj_2), side_3(traj_3) {
        metric = side_1.metr();
        tail_1 = trajectory(metric);
        tail_2 = trajectory(metric);
        tail_3 = trajectory(metric);
    }

    void preprocess() {
        if (!side_1.is_regular() || !side_2.is_regular() || !side_3.is_regular()) {
            throw std::invalid_argument("sides are not regular");
        }

        side_1.clean();
        side_2.clean();
        side_3.clean();

        auto ver_1 = *side_1.begin();
        auto ver_2 = *(--side_1.end());
        if (ver_2 == *(--side_2.end())) {
            side_2.reverse();
        }
        else if (ver_2 == *side_3.begin()) {
            swap(side_3, side_2);
        }
        else if (ver_2 == *(--side_3.end())) {
            swap(side_3, side_2);
            side_2.reverse();
        }

        if (ver_2 != *side_2.begin()) {
            throw std::invalid_argument("is not a triangle");
        }
        else {
            auto ver_3 = *(--side_2.end());
            if (ver_3 == *(--side_3.end())) {
                side_3.reverse();
            }

            if (ver_3 != *side_3.begin()) {
                throw std::invalid_argument("is not a triangle");
            }
        }
    }

    pair<list<pair<int, int>>::iterator, int> find_HS() {
        auto iter = side_1.begin();
        if (side_1.is_active_HS(iter, 1)) return make_pair(iter, 1);
        iter = side_2.begin();
        if (side_2.is_active_HS(iter, 1)) return make_pair(iter, 1);
        iter = side_3.begin();
        if (side_3.is_active_HS(iter, 1)) return make_pair(iter, 1);

        iter = (--side_1.end());
        if (side_1.is_active_HS(iter, -1)) return make_pair(iter, -1);
        iter = (--side_2.end());
        if (side_2.is_active_HS(iter, -1)) return make_pair(iter, -1);
        iter = (--side_3.end());
        if (side_3.is_active_HS(iter, -1)) return make_pair(iter, -1);



        return make_pair(iter, 0);
    }

    void bend_HS(pair<list<pair<int, int>>::iterator, int> hs) {
        auto iter = hs.first;
        auto order = hs.second;
        pair<int, int> point_1, point_2, point_3;
        auto iter_2 = iter;

        if (order == 1) {
            point_1 = *iter;
            ++iter;
            point_2 = *iter;
            iter_2 = iter;
            ++iter;
            point_3 = *iter;
        }
        else if (order == -1) {
            point_3 = *iter;
            --iter;
            point_2 = *iter;
            iter_2 = iter;
            --iter;
            point_1 = *iter;
        }
        auto d_x = point_3.first - point_2.first;
        auto d_y = point_3.second - point_2.second;
        *iter_2 = make_pair(point_1.first + d_x, point_1.second + d_y);
    }

    void zip() {
        pair<int, int> next, prev;

        if (side_1.size() > 1 && side_3.size() > 1) {
            auto ver_1 = *side_1.begin();
            next = *(++side_1.begin());
            prev = *(--(--side_3.end()));

            if (direction(ver_1, next, metric)
                == direction(ver_1, prev, metric)) {
                if (dist_square(ver_1, next) < dist_square(ver_1, prev)) {
                    tail_1.insert(ver_1);
                    side_1.erase(side_1.begin());
                    *(--side_3.end()) = next;
                }
                else if (dist_square(ver_1, next) > dist_square(ver_1, prev)) {
                    tail_1.insert(ver_1);
                    side_3.erase(--side_3.end());
                    *(side_1.begin()) = prev;
                }
                else {
                    tail_1.insert(ver_1);
                    side_3.erase(--side_3.end());
                    side_1.erase(side_1.begin());
                    zip();
                }
                side_1.clean();
                side_3.clean();
            }
        }
        if (side_2.size() > 1 && side_1.size() > 1) {
            auto ver_2 = *side_2.begin();
            next = *(++side_2.begin());
            prev = *(--(--side_1.end()));

            if (direction(ver_2, next, metric)
                == direction(ver_2, prev, metric)) {
                if (dist_square(ver_2, next) < dist_square(ver_2, prev)) {
                    tail_2.insert(ver_2);
                    side_2.erase(side_2.begin());
                    *(--side_1.end()) = next;
                }
                else if (dist_square(ver_2, next) > dist_square(ver_2, prev)) {
                    tail_2.insert(ver_2);
                    side_1.erase(--side_1.end());
                    *(side_2.begin()) = prev;
                }
                else {
                    tail_2.insert(ver_2);
                    side_1.erase(--side_1.end());
                    side_2.erase(side_2.begin());
                    zip();
                }
                side_2.clean();
                side_1.clean();
            }
        }

        if (side_3.size() > 1 && side_2.size() > 1) {
            auto ver_3 = *side_3.begin();
            next = *(++side_3.begin());
            prev = *(--(--side_2.end()));

            if (direction(ver_3, next, metric)
                == direction(ver_3, prev, metric)) {
                if (dist_square(ver_3, next) < dist_square(ver_3, prev)) {
                    tail_3.insert(ver_3);
                    side_3.erase(side_3.begin());
                    *(--side_2.end()) = next;
                }
                else if (dist_square(ver_3, next) > dist_square(ver_3, prev)) {
                    tail_3.insert(ver_3);
                    side_2.erase(--side_2.end());
                    *(side_3.begin()) = prev;
                }
                else {
                    tail_3.insert(ver_3);
                    side_2.erase(--side_2.end());
                    side_3.erase(side_3.begin());
                    zip();
                }
                side_3.clean();
                side_2.clean();
            }
        }
    }

    pair<int, int> final_triangle() {
        return make_pair(0, 0);
    }

    pair<int, int> iterative_method() {
        preprocess();

        while (true) {
            auto hs = find_HS();
            if (hs.second == 0) {
                auto t_point = final_triangle();

                tail_1.insert(*side_1.begin());
                tail_2.insert(*side_2.begin());
                tail_3.insert(*side_3.begin());

                tail_1.clean();
                tail_2.clean();
                tail_3.clean();

                return t_point;
            }
            else {
                bend_HS(hs);
                zip();
            }
        }
    }

    double sum_distance(pair<int, int> point) {
        double sum = 0;

        auto ver_1 = *side_1.begin();
        auto ver_2 = *side_2.begin();
        auto ver_3 = *side_3.begin();

        auto middle_1 = middle_HS_point(point, ver_1, metric);
        auto middle_2 = middle_HS_point(point, ver_2, metric);
        auto middle_3 = middle_HS_point(point, ver_3, metric);

        sum += sqrt(dist_square(ver_1, middle_1));
        sum += sqrt(dist_square(ver_2, middle_2));
        sum += sqrt(dist_square(ver_3, middle_3));

        sum += sqrt(dist_square(point, middle_1));
        sum += sqrt(dist_square(point, middle_2));
        sum += sqrt(dist_square(point, middle_3));

        return sum;
    }

    bool is_in_triangle(pair<int, int> point) {
        return true;
    }

    vector<pair<int, int>> gradient_method(int R = 16, int N = 100) {
        
        vector<pair<int, int>> points;
        points.push_back(* side_3.begin());

        if (metric == 2) {
            auto p = points.back();
            int cur_min_sum = sum_distance(p);
            for (int count = 0; count < N; ++count) {
                auto p = points.back();

                pair<int, int> candidates[4];
                candidates[0] = make_pair(p.first, p.second + R);
                candidates[1] = make_pair(p.first + R, p.second);
                candidates[2] = make_pair(p.first, p.second - R);
                candidates[3] = make_pair(p.first - R, p.second);

                bool is_found = false;
                for (int i = 0; i < 4; ++i) {
                    int cur_sum = sum_distance(candidates[i]);
                    if (cur_min_sum > cur_sum) {
                        p = candidates[i];
                        cur_min_sum = cur_sum;
                        is_found = true;
                    }
                }

                if (is_found) {
                    points.push_back(p);
                }
                else {
                    if (R == 1) break;
                    else R = R / 2;
                }
            }
        }

        else if (metric == 4) {
            auto p = points.back();
            double cur_min_sum = sum_distance(p);
            for (int count = 0; count < N; ++count) {
                auto p = points.back();

                vector<pair<int, int>> candidates;
                vector<pair<int, int>> moves = { {0, R}, {R, R}, {R, 0}, {R, -R}, {0, -R}, {-R, -R}, {-R, 0}, {-R, R} };
                for (auto move : moves) {
                    candidates.emplace_back(p.first + move.first, p.second + move.second);
                }

                bool is_found = false;
                for (int i = 0; i < 7; ++i) {
                    if (is_in_triangle(candidates[i])) {
                        double cur_sum = sum_distance(candidates[i]);
                        if (cur_min_sum > cur_sum) {
                            p = candidates[i];
                            cur_min_sum = cur_sum;
                            is_found = true;
                        }
                    }
                }

                if (is_found) {
                    points.push_back(p);
                }
                else {
                    if (R == 1) break;
                    else R = R / 2;
                }
            }
        }
        else {
            throw std::invalid_argument("wrong metric!");
        }
        return points;
    }
};

void print(vector<pair<int, int>>::iterator begin, vector<pair<int, int>>::iterator end, string file_name) {
    ofstream cout(file_name);
    for (auto iter = begin; iter != end; ++iter) {
        auto point = *iter;
        cout << point.first << ", " << point.second << endl;
    }
}

int main() {

    trajectory a = trajectory(2);
    trajectory b = trajectory(2);
    trajectory c = trajectory(2);

    int x = -1;
    int y = 0;
    for (int i = 0; i <= 100; ++i) {
        if (i % 2 == 0) {
            x++;
        }
        else {
            y++;
        }
        a.insert(make_pair(x, y));
    }
    x--;

    for (int i = 0; i <= 200; ++i) {
        if (i % 2 == 0) {
            x++;
        }
        else {
            y--;
        }
        b.insert(make_pair(x, y));
    }
    pair<int, int> ver_1 = make_pair(0, y);
    pair<int, int> ver_2 = make_pair(x, y);

    c.insert(make_pair(0, 0));
    c.insert(ver_1);
    c.insert(ver_2);

    triangle T = triangle(a, b, c);
    T.preprocess();

    T.side_1.print("side_1.txt");
    T.side_2.print("side_2.txt");
    T.side_3.print("side_3.txt");

    auto points = T.gradient_method();

    /*T.tail_1.print("tail_1.txt");
    T.tail_2.print("tail_2.txt");
    T.tail_3.print("tail_3.txt");

    T.side_1.print("side_1_new.txt");
    T.side_2.print("side_2_new.txt");
    T.side_3.print("side_3_new.txt");*/


    print(points.begin(), points.end(), "gradient.txt");

}