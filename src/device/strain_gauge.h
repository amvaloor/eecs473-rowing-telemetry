// strain_gauge.h
// TODO once the specific model is chosen, this file should be renamed to the
// name of the model
#pragma once

class StrainGauge {
private:
  // TODO pin numbers
public:
  /**
   * @brief Creates a StrainGauge object
   *
   * @ return A StrainGauge object
   */
  StrainGauge(/* TODO relevant pins go here */);

  /**
   * @brief Gets the current force
   *
   * @return The current force in N
   */
  int get_force();
};
