#include <time_shield/time_utils.hpp>

/// \brief Verify library usage from an installed location.
int main() {
    const auto current_ts = time_shield::now();
    return current_ts > 0 ? 0 : 1;
}
