#ifndef STATISTICS
#define STATISTICS

namespace stats {
class Mean {
  int N_{};
  double sum_x_{};
  double sum_w_x_{};
  double sum_w_{};

 public:
  int size() const;
  void add(double value, double weight);
  void add(double value);
  double mean() const;
  double w_mean() const;
};

inline int Mean::size() const { return N_; }
inline void Mean::add(double value, double weight) {
  if (weight <= 0) throw std::runtime_error{"Weight can't be zero"};
  sum_w_x_ += value * weight;
  sum_x_ += value;
  sum_w_ += weight;
  N_++;
}
inline void Mean::add(double value) {
  sum_x_ += value;
  N_++;
}
inline double Mean::mean() const {
  if (N_ == 0) throw std::runtime_error{"Can't calculate mean without values"};
  return (sum_x_ / N_);
}
inline double Mean::w_mean() const {
  //   assert(sum_w != 0);
  if (N_ == 0)
    throw std::runtime_error{"Can't calculate weighted mean without values"};

  return (sum_w_x_ / sum_w_);
}

struct Result {
  double A;
  double B;
};

class Regression {
  int N_{};
  double sum_x_{};
  double sum_y_{};
  double sum_xy_{};
  double sum_x2_{};

 public:
  int size() const;
  void add(double x, double y);
  Result fit() const;
  Result get_sigma(double sigma_y) const;
};

inline int Regression::size() const { return N_; }
inline void Regression::add(double x, double y) {
  ++N_;
  sum_x_ += x;
  sum_y_ += y;
  sum_xy_ += x * y;
  sum_x2_ += x * x;
}
inline Result Regression::fit() const {
  if (N_ < 2) {
    throw std::runtime_error{"Not enough points to run a fit"};
  }
  double const d = N_ * sum_x2_ - sum_x_ * sum_x_;
  if (d == 0.) {
    throw std::runtime_error{"Trying to fit a vertical line"};
  }
  double const a = (sum_y_ * sum_x2_ - sum_x_ * sum_xy_) / d;
  double const b = (N_ * sum_xy_ - sum_x_ * sum_y_) / d;
  return {a, b};
}
inline Result Regression::get_sigma(double sigma_y) const {
  double const d = N_ * sum_x2_ - sum_x_ * sum_x_;
  double const sigma_a = sigma_y * sqrt(sum_x2_ / d);
  double const sigma_b = sigma_y * sqrt(N_ / d);
  return {sigma_a, sigma_b};
}
}  // namespace stats

#endif