#include "bragg.h"
#include "error_handling.h"

// we use the same values as the DIRAC15 code
double get_bragg_angstrom(const int charge)
{
    double retval = 0.0;
    std::ostringstream err_msg;
    err_msg << "Bragg-Angstrom radius not found for element ";
    switch (charge)
    {
    case 1:
        retval = 0.35;
        break;
    case 2:
        retval = 0.35;
        break;
    case 3:
        retval = 1.45;
        break;
    case 4:
        retval = 1.05;
        break;
    case 5:
        retval = 0.85;
        break;
    case 6:
        retval = 0.70;
        break;
    case 7:
        retval = 0.65;
        break;
    case 8:
        retval = 0.60;
        break;
    case 9:
        retval = 0.50;
        break;
    case 10:
        retval = 0.45;
        break;
    case 11:
        retval = 1.80;
        break;
    case 12:
        retval = 1.50;
        break;
    case 13:
        retval = 1.25;
        break;
    case 14:
        retval = 1.10;
        break;
    case 15:
        retval = 1.00;
        break;
    case 16:
        retval = 1.00;
        break;
    case 17:
        retval = 1.00;
        break;
    case 18:
        retval = 1.00;
        break;
    case 19:
        retval = 2.20;
        break;
    case 20:
        retval = 1.80;
        break;
    case 21:
        retval = 1.60;
        break;
    case 22:
        retval = 1.40;
        break;
    case 23:
        retval = 1.35;
        break;
    case 24:
        retval = 1.40;
        break;
    case 25:
        retval = 1.40;
        break;
    case 26:
        retval = 1.40;
        break;
    case 27:
        retval = 1.35;
        break;
    case 28:
        retval = 1.35;
        break;
    case 29:
        retval = 1.35;
        break;
    case 30:
        retval = 1.35;
        break;
    case 31:
        retval = 1.30;
        break;
    case 32:
        retval = 1.25;
        break;
    case 33:
        retval = 1.15;
        break;
    case 34:
        retval = 1.15;
        break;
    case 35:
        retval = 1.15;
        break;
    case 36:
        retval = 1.10;
        break;
    case 37:
        retval = 2.35;
        break;
    case 38:
        retval = 2.00;
        break;
    case 39:
        retval = 1.80;
        break;
    case 40:
        retval = 1.55;
        break;
    case 41:
        retval = 1.45;
        break;
    case 42:
        retval = 1.45;
        break;
    case 43:
        retval = 1.35;
        break;
    case 44:
        retval = 1.30;
        break;
    case 45:
        retval = 1.35;
        break;
    case 46:
        retval = 1.40;
        break;
    case 47:
        retval = 1.60;
        break;
    case 48:
        retval = 1.55;
        break;
    case 49:
        retval = 1.55;
        break;
    case 50:
        retval = 1.45;
        break;
    case 51:
        retval = 1.45;
        break;
    case 52:
        retval = 1.40;
        break;
    case 53:
        retval = 1.40;
        break;
    case 54:
        retval = 1.40;
        break;
    case 55:
        retval = 2.60;
        break;
    case 56:
        retval = 2.15;
        break;
    case 57:
        retval = 1.95;
        break;
    case 58:
        retval = 1.85;
        break;
    case 59:
        retval = 1.85;
        break;
    case 60:
        retval = 1.85;
        break;
    case 61:
        retval = 1.85;
        break;
    case 62:
        retval = 1.85;
        break;
    case 63:
        retval = 1.85;
        break;
    case 64:
        retval = 1.80;
        break;
    case 65:
        retval = 1.75;
        break;
    case 66:
        retval = 1.75;
        break;
    case 67:
        retval = 1.75;
        break;
    case 68:
        retval = 1.75;
        break;
    case 69:
        retval = 1.75;
        break;
    case 70:
        retval = 1.75;
        break;
    case 71:
        retval = 1.75;
        break;
    case 72:
        retval = 1.55;
        break;
    case 73:
        retval = 1.45;
        break;
    case 74:
        retval = 1.35;
        break;
    case 75:
        retval = 1.30;
        break;
    case 76:
        retval = 1.30;
        break;
    case 77:
        retval = 1.35;
        break;
    case 78:
        retval = 1.35;
        break;
    case 79:
        retval = 1.35;
        break;
    case 80:
        retval = 1.50;
        break;
    case 81:
        retval = 1.90;
        break;
    case 82:
        retval = 1.75;
        break;
    case 83:
        retval = 1.60;
        break;
    case 84:
        retval = 1.90;
        break;
    case 85:
        retval = 1.50;
        break;
    case 86:
        retval = 1.50;
        break;
    default:
        err_msg << charge;
        NUMGRID_ERROR(err_msg);
        break;
    }
    return retval;
}
