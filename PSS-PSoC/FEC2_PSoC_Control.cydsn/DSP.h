/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================*/
#include <project.h>
#include <math.h>

/**************/
/*  Defines   */
/**************/

/*************/
/*  Macros   */
/*************/
#define abs(x)           (((x) < 0) ? -(x) : (x))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define CYC_INC_INDX(i,N) (((i) < (N-1)) ? (i+1) : (0))

#define PI 3.141592654
#define OMEGA 2*PI

/*
 * The following routines have been built from knowledge gathered
 * around the Web. I am not aware of any copyright problem with
 * them, so use it as you want.
 * N. Devillard - 1998
 */

#define PAIR_SORT(a,b) { if ((a)>(b)) PAIR_SWAP((a),(b)); }
#define PAIR_SWAP(a,b) { int16 temp=(a);(a)=(b);(b)=temp; }

/*----------------------------------------------------------------------------
   Function :   opt_med3()
   In       :   pointer to array of 3 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 3 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt3(int16 * p);


/*----------------------------------------------------------------------------
   Function :   opt_med5()
   In       :   pointer to array of 5 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 5 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt5(int16 * p);


/*----------------------------------------------------------------------------
   Function :   opt_med6()
   In       :   pointer to array of 6 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 6 pixel values
   Notice   :   from Christoph_John@gmx.de
                based on a selection network which was proposed in
                "FAST, EFFICIENT MEDIAN FILTERS WITH EVEN LENGTH WINDOWS"
                J.P. HAVLICEK, K.A. SAKADY, G.R.KATZ
                If you need larger even length kernels check the paper
 ---------------------------------------------------------------------------*/
int16 medfilt6(int16 * p);


/*----------------------------------------------------------------------------
   Function :   opt_med7()
   In       :   pointer to array of 7 pixel values
   Out      :   a int16
   Job      :   optimized search of the median of 7 pixel values
   Notice   :   found on sci.image.processing
                cannot go faster unless assumptions are made
                on the nature of the input signal.
 ---------------------------------------------------------------------------*/
int16 medfilt7(int16 * p);

/*----------------------------------------------------------------------------
   Function :   opt_med9()
   In       :   pointer to an array of 9 int16s
   Out      :   a int16
   Job      :   optimized search of the median of 9 int16s
   Notice   :   in theory, cannot go faster without assumptions on the
                signal.
                Formula from:
                XILINX XCELL magazine, vol. 23 by John L. Smith
  
                The input array is modified in the process
                The result array is guaranteed to contain the median
                value
                in middle position, but other elements are NOT sorted.
 ---------------------------------------------------------------------------*/
int16 medfilt9(int16 * p);


/*----------------------------------------------------------------------------
   Function :   opt_med25()
   In       :   pointer to an array of 25 int16s
   Out      :   a int16
   Job      :   optimized search of the median of 25 int16s
   Notice   :   in theory, cannot go faster without assumptions on the
                signal.
  				Code taken from Graphic Gems.
 ---------------------------------------------------------------------------*/
int16 medfilt25(int16 * p);


//#undef PAIR_SORT
//#undef PAIR_SWAP




/*******************************************************************************
* Function Name: int16 * sort(int16 * ArrayIn)
********************************************************************************
* Summary: Bobble Sort algorithm
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void sort(int16 * ArrayIn, uint8 N);

/*******************************************************************************
* Function Name: int16 Med_Filter(int16 * ArrayIn, uint8 W)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
int16 Med_Filter(int16 * ArrayIn, uint8 W);


/*******************************************************************************
* Function Name: float SingelPole_IIR_LPF(float x, float y, float Fc, float Fs);
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/

    /* IIR filter:

     * Difference equation of 1-pole IIR
     y[k+1] = y[k] + (x[k]-y[k])/a

     * The Z transform of the transfer function:
     H(z)=1/[a+z^(-1)*(1-a)]

     * The cutoff frequency (Fo) at a sampling rate Fs:
     Fo = Fs / 2*(pi)*a

     example:
     a = 256/1
     Fs = 100Hz
     Fo = 100 /2*(pi)*256

     **************
     ** Fo = 0.06Hz **
     **************

     * Settling Time (Ts):
     s[n] = 1 - (1- 1/a )^n
     n = ln(0.001) / ln(1-1/a)
     n = 1765
     Ts = n/Fs

     ********************
     ** Ts = = 17.6sec **
     ********************
     */
	//IIR_FILTER_NUMERATOR = round(512*PI*Fc/Fs)
	// Final Fc = Fs/(2*PI*IIR_FILTER_NUMERATOR)
int32 IIR_LPF_sp(int32 input, int32 * filt, uint8 Numerator);

/*******************************************************************************
* Function Name: Shift_And_Store_int16(int16 * pArray, uint8 SizeOfArray, int16 NewData, char ShiftDirection);
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
void Shift_And_Store_int16(int16 * pArray, uint8 SizeOfArray, int16 NewData, char ShiftDirection);

/*******************************************************************************
* Function Name: uint16 findpeaks(uint16 * Data, uint8 N, uint8 aw)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.
*******************************************************************************/
uint16 findpeaks(uint16 * Data, uint8 N, uint8 aw);

/*******************************************************************************
* Function Name: int16 mean(int16 * Data, uint8 N)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.ADC_CountsTo_Volts()
*******************************************************************************/
int16 mean(int16 * Data, uint8 N);

/*******************************************************************************
* Function Name: int16 RangeMean(int16 * Data, uint8 N, uint8 R)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   none.ADC_CountsTo_Volts()
*******************************************************************************/
int16 RangeMean(int16 * Data, uint8 N, uint8 R);

/*******************************************************************************
* Function Name: uint16 accum(uint16 * Data, uint8 N)
********************************************************************************
* Summary:
*
* Parameters:
*   none.
*
* Return:
*   
*******************************************************************************/
uint16 accum(uint16 * Data, uint8 N);

/* [] END OF FILE */

