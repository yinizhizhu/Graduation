#include "batree.cpp"

int main() {
	cout << "Main thread_id: " << this_thread::get_id() << "\n";
	batree<int> r;
	r.fastRandom();
	return 0;
}