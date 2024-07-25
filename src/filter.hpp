#pragma once

#include "matrix.hpp"
namespace ln {
	class Scene;
	class Filter
	{
	public:
		virtual std::pair<vector, bool> filter(const vector&) const = 0;
		virtual ~Filter() = default;
	};

	struct ClipFilter : public Filter
	{
		ClipFilter(matrix, vector, Scene&);
		matrix m_;
		vector eye_;
		Scene& scene_;
		~ClipFilter() override;
		std::pair<vector, bool> filter(const vector&) const override;
	};

} // namespace ln