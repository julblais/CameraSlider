#ifndef CONFIG_H
#define CONFIG_H

#ifdef IS_SIMULATOR

#define C_DpadUpPin             23
#define C_DpadDownPin           34
#define C_DpadLeftPin           16
#define C_DpadRightPin          19
#define C_DpadSelectionPin      18
#define C_JoystickXPin          35
#define C_JoystickYPin          39
#define C_JoystickCenterPin     17
#define C_StepperStepPin        33
#define C_StepperDirectionPin   32
#define C_LcdAddress        0x27
#define C_ShowMenuDelayMs   2000

#else

#define C_DpadUpPin             23
#define C_DpadDownPin           34
#define C_DpadLeftPin           16
#define C_DpadRightPin          19
#define C_DpadSelectionPin      18
#define C_JoystickXPin          35
#define C_JoystickYPin          39
#define C_JoystickCenterPin     17
#define C_StepperStepPin        33
#define C_StepperDirectionPin   32
#define C_LcdAddress            0x27
#define C_ShowMenuDelayMs       2000

#endif
#endif // CONFIG_H
