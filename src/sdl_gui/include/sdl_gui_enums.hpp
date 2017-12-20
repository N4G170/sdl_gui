namespace sdl_gui
{
#ifndef SDL_GUI_ENUMS
#define SDL_GUI_ENUMS

//<f> GuiElements
enum class AnchorType
{
    TOP_LEFT,
    TOP_CENTRE,
    TOP_RIGHT,
    MIDDLE_LEFT,
    MIDDLE_CENTRE,
    MIDDLE_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_CENTRE,
    BOTTOM_RIGHT
};
// </f>

//<f> Interaction
enum class ButtonTransitionType
{
    NONE,           ///< No transition
    COLOUR,         ///< Change button colour
    SINGLE_IMAGE,   ///< Change button image. One image were we select the rect to render
    MULTI_IMAGE     ///< Change button image. Ome image per state
};

/**
 * \brief Button transition states
 */
enum class ButtonState
{
    INACTIVE,   ///< Button is inactive, cannot be interacted with
    ACTIVE,     ///< Button is active, can be interacted with
    OVER,       ///< Button has "something"(usually mouse) hovering it
    PRESSED,    ///< Button was pressed
    SELECTED    ///< Button "remains" pressed
};

/**
 * \brief Base mouse interaction flags
 */
enum class MouseStateFlags
{
    NONE = 0,   ///< Clear state
    ENTER = 1,  ///< Mouse entered the detection area
    EXIT = 2,   ///< Mouse left the detection area
    OVER = 4,   ///< Mouse is inside the detection area
    MOVE = 8    ///< Mouse moved
};
/**
 * \brief Mouse button interaction state flags
 */
enum class InputKeyStateFlags
{
    NONE = 0,     ///< Empty state
    UP = 1,       ///< Key up
    DOWN = 2,     ///< Key down
    HOLD = 4,     ///< Key holding down
};

/**
 * \brief Mouse actions available for use with callbacks
 */
enum class MouseCallbackType
{
    ENTER,      ///< Mouse entered detection area
    OVER,       ///< Mouse is inside detection area
    EXIT,       ///< Mouse left detection area
    MOVE,       ///< Mouse moved
};
/**
 * \brief Mouse button actions available for use with callbacks
 */
enum class InputKeyCallbackType
{
    DOWN,   ///< Mouse button down
    HOLD,   ///< Mouse button holding down
    CLICK,  ///< Mouse button clicked
    UP,     ///< Mouse button up
};

enum class TextboxInputType
{
    ALPHANUMERIC,
    INTEGER,
    PASSWORD, ///< will show *****
};

// </f>

//<f> Layout
enum class LayoutWrapMode
{
    LAYOUT_OVERFLOW,//OVERFLOW already defined in cmath and the compiler complains so added LAYOUT_
    LAYOUT_HIDDEN
};
//</f>

//<f> SelectableGroup
enum class SelectableGroupType
{
    MULTIPLE,
    RADIO,
};
//</f>

//<f> ProgressBar & Sliders
enum class ProgressBarDirection
{
    LEFT,   ///< Bar fills right->left
    RIGHT,  ///< Bar fills left->right
    UP,     ///< Bar fills bottom->up
    DOWN    ///< Bar fills top->down
};

enum class SliderDirection
{
    LEFT,   ///< Bar fills right->left
    RIGHT,  ///< Bar fills left->right
    UP,     ///< Bar fills bottom->up
    DOWN    ///< Bar fills top->down
};
//</f>
#endif// SDL_GUI_ENUMS
}//namespace
