#include <xc.h>
#include "buttons.h"
#include "configuration.h"
#include <stdbool.h>


/*************************************************************
 *
 * de_bounce (http://www.dattalo.com/technical/software/pic/debounce.html)
 *
 * The purpose of this routine is to debounce, i.e. digitally low pass filter
 * inputs. The algorithm handles upto 8 bits at a time. An input is considered
 * filtered if it has not changed states in the last 4 samples.
 *
 *  2-bit cyclic vertical counters count the 4 samples. As long as there is no
 * change, the counters are held in the reset state of 00b. When a change is detected
 * between the current sample and the filtered or debounced sample, the counters
 * are incremented. The counting sequence is 00,01,10,11,00... When the counters
 * roll over from 11b to 00b, the debounced state is updated. If the input changes
 * back to the filtered state while the counters are counting, then the counters
 * are re-initialized to the reset state and the filtered state is unaffected.
 * In other words, a glitch or transient input has been filtered.
 *
 * The algorithm will return in W the state of those inputs that have just
 * been filtered.
 */

static unsigned char clock_A,clock_B,debounced_state;
unsigned char debounce(unsigned char new_sample)
{
  unsigned char delta;
  unsigned char changes;

  delta = new_sample ^ debounced_state;   //Find all of the changes

  clock_A ^= clock_B;                     //Increment the counters
  clock_B  = ~clock_B;

  clock_A &= delta;                       //Reset the counters if no changes
  clock_B &= delta;                       //were detected.

  changes = ~(~delta | clock_A | clock_B);
  debounced_state ^= changes;

  return changes;
}


static unsigned char current_status;
void updateButtonReadings()
{
    unsigned char temp;
    temp = (BUTTON1_PIN == 0 ? 1 : 0);
    temp |= (BUTTON2_PIN == 0 ? 1 : 0) << 1;
    current_status = temp; //debounce(temp);
}

static bool button1_not_read;
bool button1Pressed()
{
//    todo: button_not_read moet na n ms weer gereset worden

    bool temp = button1_not_read && (current_status & (1));
    button1_not_read = !(current_status & (1));
    return temp;
}

static bool button2_not_read;
bool button2Pressed()
{
    bool temp = button2_not_read && (current_status & (1 << 1));
    button2_not_read = !(current_status & (1 << 1));
    return temp;
//    return (current_status & (1 << 1));
}

