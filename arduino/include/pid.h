const float wheel_base = 0.2;
const float max_speed = 255;

// PID Constants (Tune These)
float kp_angle = 1.5, ki_angle = 0.0, kd_angle = 0.1; // For angle control
float kp_distance = 1.2, ki_distance = 0.0, kd_distance = 0.1; // For distance control

// Robot State
float left_speed = 0, right_speed = 0;                // Motor speeds

// Timing for PID
unsigned long last_time = 0;
float angle_error_integral = 0, distance_error_integral = 0;

void pidControl() {
    // Timing
    unsigned long current_time = millis();
    float dt = (current_time - last_time) / 1000.0; // Time delta in seconds
    last_time = current_time;

    // Calculate Target Angle and Distance
    float theta_desired = atan2(target.y - y, target.x - x);
    float distance_target = sqrt(pow(target.x - x, 2) + pow(target.y - y, 2));

    // Calculate Errors
    float angle_error = theta_desired - theta;
    float distance_error = distance_target;

    // Normalize Angle Error (-PI to PI)
    if (angle_error > PI) angle_error -= 2 * PI;
    if (angle_error < -PI) angle_error += 2 * PI;

    // PID for Angle
    angle_error_integral += angle_error * dt;
    float angle_derivative = angle_error / dt;
    float angle_control = kp_angle * angle_error + ki_angle * angle_error_integral + kd_angle * angle_derivative;

    // PID for Distance
    distance_error_integral += distance_error * dt;
    float distance_derivative = distance_error / dt;
    float distance_control = kp_distance * distance_error + ki_distance * distance_error_integral + kd_distance * distance_derivative;

    // Calculate Motor Speeds
    left_speed = distance_control - angle_control * wheel_base / 2.0;
    right_speed = distance_control + angle_control * wheel_base / 2.0;

    // Constrain Speeds
    left_speed = constrain(left_speed, -max_speed, max_speed);
    right_speed = constrain(right_speed, -max_speed, max_speed);

    // Set Motor Speeds
    setMotorSpeeds(left_speed, right_speed);
}

void setMotorSpeeds(float left, float right) {
    // give values to motor controller
}