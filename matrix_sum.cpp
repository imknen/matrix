#include "test_runner.h"
#include <vector>
#include <future>
#include <numeric>
#include <list>
using namespace std;


template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
    , size_(distance(first, last))
  {
  }
  Iterator begin() const {
    return first;
  }
  Iterator end() const {
    return last;
  }
  size_t size() const {
    return size_;
  }
private:
  Iterator first, last;
  size_t size_;
};

template <typename Iterator>
class Paginator {
private:
  vector<IteratorRange<Iterator>> pages;
public:
  Paginator(Iterator begin, Iterator end, size_t page_size) {
    for (size_t left = distance(begin, end); left > 0; ) {
      size_t current_page_size = min(page_size, left);
      Iterator current_page_end = next(begin, current_page_size);
      pages.push_back({begin, current_page_end});
      left -= current_page_size;
      begin = current_page_end;
    }
  }
  auto begin() const {
    return pages.begin();
  }
  auto end() const {
    return pages.end();
  }
  size_t size() const {
    return pages.size();
  }
};
template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(begin(c), end(c), page_size);
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  // Реализуйте эту функцию
	list<future<int64_t>> f_res;
	for (auto page : Paginate(matrix, 2000) )
	{
		f_res.push_back(async([page]{
			int64_t res = 0;
			for (auto & vec : page) {
				for (auto& i : vec) {
					res += i;
				}
			}
			return res;
			})
			);
	}
	int64_t ret = 0;
	for (auto& f : f_res)
	{
		ret += f.get();
	}
	return ret;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
