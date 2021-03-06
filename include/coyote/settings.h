﻿// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#ifndef COYOTE_CONFIGURATION_H
#define COYOTE_CONFIGURATION_H

#include <chrono>
#include <stdexcept>
#include "strategies/strategy_type.h"

namespace coyote
{
	class Settings
	{
	private:
		// The execution exploration strategy.
		StrategyType strategy_type;

		// A strategy-specific bound.
		size_t strategy_bound;

		// The seed used by randomized strategies.
		uint64_t seed_state;

	public:
		Settings() noexcept :
			strategy_type(StrategyType::Random),
			seed_state(std::chrono::high_resolution_clock::now().time_since_epoch().count())
		{
		}

		Settings(Settings&& strategy) = delete;
		Settings(Settings const&) = delete;

		Settings& operator=(Settings&& strategy) = delete;
		Settings& operator=(Settings const&) = delete;

		// Installs the random exploration strategy with the specified random seed.
		void use_random_strategy(uint64_t seed) noexcept
		{
			strategy_type = StrategyType::Random;
			seed_state = seed;
			strategy_bound = 100;
		}

		// Installs the random exploration strategy with the specified random seed and probability
		// of deviating from the currently scheduled enabled operation.
		void use_random_strategy(uint64_t seed, size_t probability)
		{
			if ( probability > 100) {
				throw std::invalid_argument("received probability greater than 100");
			}

			strategy_type = StrategyType::Random;
			seed_state = seed;
			strategy_bound = probability;
		}

		// Installs the PCT exploration strategy with the specified random seed and priority switch bound.
		void use_pct_strategy(uint64_t seed, size_t bound) noexcept
		{
			strategy_type = StrategyType::PCT;
			seed_state = seed;
			strategy_bound = bound;
		}

		// Disables controlled scheduling.
		void disable_scheduling() noexcept
		{
			strategy_type = StrategyType::None;
		}

		// Returns the type of the installed exploration strategy.
		StrategyType exploration_strategy() noexcept
		{
			return strategy_type;
		}

		// Returns an exploration strategy specific bound.
		size_t exploration_strategy_bound() noexcept
		{
			return strategy_bound;
		}

		// Returns the seed used by randomized strategies.
		uint64_t random_seed() noexcept
		{
			return seed_state;
		}
	};
}

#endif // COYOTE_CONFIGURATION_H
