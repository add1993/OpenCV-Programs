// FILE: tripodDlg.cpp : implementation file
// AUTH: P. Oh
// DATE: 04/24/02 04:00
// VERS: 11/21/01 : 0.9 posted on web
//       04/24/02 : 1.0 Added free to release memory malloced for m_destinationBmp
// DESC: TRIPOD template files for real-time image processing development
// STAT: 1.0 Working
// REFS: brinarize - Example application for binary images

#include "stdafx.h"
#include "tripod.h"
#include "tripodDlg.h"

#include "LVServerDefs.h"
#include "math.h"
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTripodDlg dialog

CTripodDlg::CTripodDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTripodDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTripodDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//////////////// Set destination BMP to NULL first 
	m_destinationBitmapInfoHeader = NULL;
	m_destinationBitmapRightInfoHeader = NULL;

}

////////////////////// Additional generic functions

static unsigned PixelBytes(int w, int bpp)
{
    return (w * bpp + 7) / 8;
}

static unsigned DibRowSize(int w, int bpp)
{
    return (w * bpp + 31) / 32 * 4;
}

static unsigned DibRowSize(LPBITMAPINFOHEADER pbi)
{
    return DibRowSize(pbi->biWidth, pbi->biBitCount);
}

static unsigned DibRowPadding(int w, int bpp)
{
    return DibRowSize(w, bpp) - PixelBytes(w, bpp);
}

static unsigned DibRowPadding(LPBITMAPINFOHEADER pbi)
{
    return DibRowPadding(pbi->biWidth, pbi->biBitCount);
}

static unsigned DibImageSize(int w, int h, int bpp)
{
    return h * DibRowSize(w, bpp);
}

static size_t DibSize(int w, int h, int bpp)
{
    return sizeof (BITMAPINFOHEADER) + DibImageSize(w, h, bpp);
}

/////////////////////// end of generic functions


void CTripodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTripodDlg)
	DDX_Control(pDX, IDC_PROCESSEDVIEW, m_cVideoProcessedView);
	DDX_Control(pDX, IDC_UNPROCESSEDVIEW, m_cVideoUnprocessedView);
	DDX_Control(pDX, IDC_PROCESSEDVIEWRIGHT, m_cVideoProcessedViewRight);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTripodDlg, CDialog)
	//{{AFX_MSG_MAP(CTripodDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDEXIT, OnExit)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTripodDlg message handlers

BOOL CTripodDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	countStart = 0;
	
	
	// Variables for using 2 cameras
	whichSide = false;
	wasDisplayed = false;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// For Unprocessed view videoportal (top one)
	char sRegUnprocessedView[] = "HKEY_LOCAL_MACHINE\\Software\\UnprocessedView";
	m_cVideoUnprocessedView.PrepareControl("UnprocessedView", sRegUnprocessedView, 0 );	
	m_cVideoUnprocessedView.EnableUIElements(UIELEMENT_STATUSBAR,0,TRUE);
	m_cVideoUnprocessedView.ConnectCamera(0);
	m_cVideoUnprocessedView.SetEnablePreview(TRUE);

	// For binary view videoportal (bottom one)
	char sRegProcessedView[] = "HKEY_LOCAL_MACHINE\\Software\\ProcessedView";
	m_cVideoProcessedView.PrepareControl("ProcessedView", sRegProcessedView, 0 );	
	m_cVideoProcessedView.EnableUIElements(UIELEMENT_STATUSBAR,0,TRUE);
	m_cVideoProcessedView.ConnectCamera(0);
	m_cVideoProcessedView.SetEnablePreview(TRUE);

	// Initialize the size of binary videoportal
	m_cVideoProcessedView.SetPreviewMaxHeight(240);
	m_cVideoProcessedView.SetPreviewMaxWidth(320);

	// For binary view videoportal (bottom one)
	char sRegProcessedViewRight[] = "HKEY_LOCAL_MACHINE\\Software\\ProcessedViewRight";
	m_cVideoProcessedViewRight.PrepareControl("ProcessedViewReview", sRegProcessedViewRight, 0 );	
	m_cVideoProcessedViewRight.EnableUIElements(UIELEMENT_STATUSBAR,0,TRUE);
	m_cVideoProcessedViewRight.ConnectCamera(0);
	m_cVideoProcessedViewRight.SetEnablePreview(TRUE);

	// Initialize the size of binary videoportal
	m_cVideoProcessedViewRight.SetPreviewMaxHeight(240);
	m_cVideoProcessedViewRight.SetPreviewMaxWidth(320);

	// Uncomment if you wish to fix the live videoportal's size
	// m_cVideoUnprocessedView.SetPreviewMaxHeight(240);
	// m_cVideoUnprocessedView.SetPreviewMaxWidth(320);

	// Find the screen coodinates of the binary videoportal
	m_cVideoProcessedView.GetWindowRect(m_rectForProcessedView);
	ScreenToClient(m_rectForProcessedView);
	allocateDib(CSize(320, 240));

	// Find the screen coodinates of the binary videoportal
	m_cVideoProcessedViewRight.GetWindowRect(m_rectForProcessedViewRight);
	ScreenToClient(m_rectForProcessedViewRight);
	allocateDib(CSize(320, 240));

	// Start grabbing frame data for Procssed videoportal (bottom one)
	m_cVideoProcessedView.StartVideoHook(0);

	// Start grabbing frame data for Procssed videoportal (bottom one)
	m_cVideoProcessedViewRight.StartVideoHook(0);
	
	SetTimer(1, 540, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control

	
}

void CTripodDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTripodDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}

}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTripodDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTripodDlg::OnExit() 
{
	// TODO: Add your control notification handler code here

	// Kill live view videoportal (top one)
	m_cVideoUnprocessedView.StopVideoHook(0);
    m_cVideoUnprocessedView.DisconnectCamera();	
	
	// Kill binary view videoportal (bottom left one)
	m_cVideoProcessedView.StopVideoHook(0);
    m_cVideoProcessedView.DisconnectCamera();	

	// Kill binary view videoportal (bottom right one)
	m_cVideoProcessedViewRight.StopVideoHook(0);
    m_cVideoProcessedViewRight.DisconnectCamera();

	// Kill program
	DestroyWindow();	

}

BEGIN_EVENTSINK_MAP(CTripodDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CTripodDlg)
	ON_EVENT(CTripodDlg, IDC_PROCESSEDVIEW, 1 /* PortalNotification */, OnPortalNotificationProcessedview, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CTripodDlg, IDC_PROCESSEDVIEWRIGHT, 1 /* PortalNotification */, OnPortalNotificationProcessedviewRight, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CTripodDlg::OnPortalNotificationProcessedview(long lMsg, long lParam1, long lParam2, long lParam3) 
{
	// TODO: Add your control notification handler code here
	
	// This function is called at the camera's frame rate
    
#define NOTIFICATIONMSG_VIDEOHOOK	10

	// Declare some useful variables
	// QCSDKMFC.pdf (Quickcam MFC documentation) p. 103 explains the variables lParam1, lParam2, lParam3 too 


	//LPBITMAPINFOHEADER lpBitmapInfoHeader; // Frame's info header contains info like width and height
	//LPBYTE lpBitmapPixelData; // This pointer-to-long will point to the start of the frame's pixel data
    //unsigned long lTimeStamp; // Time when frame was grabbed

	switch(lMsg) {
		case NOTIFICATIONMSG_VIDEOHOOK:
			{
				if(whichSide == 0)
				{
					lpBitmapInfoHeader = (LPBITMAPINFOHEADER) lParam1; 
					lpBitmapPixelData = (LPBYTE) lParam2;
					lTimeStamp = (unsigned long) lParam3;
	
					//grayScaleTheFrameData(lpBitmapInfoHeader, lpBitmapPixelData);
					//doMyImageProcessing(lpBitmapInfoHeader); // Place where you'd add your image processing code
					//displayMyResults(lpBitmapInfoHeader);
				}
			}
			break;

		default:
			break;
	}	

	wasDisplayed = true;
}

void CTripodDlg::OnPortalNotificationProcessedviewRight(long lMsg, long lParam1, long lParam2, long lParam3) 
{
	// TODO: Add your control notification handler code here
	
	// This function is called at the camera's frame rate
    
#define NOTIFICATIONMSG_VIDEOHOOK2	10

	// Declare some useful variables
	// QCSDKMFC.pdf (Quickcam MFC documentation) p. 103 explains the variables lParam1, lParam2, lParam3 too 
	

	switch(lMsg) {
		case NOTIFICATIONMSG_VIDEOHOOK2:
			{
				if(whichSide == 1)
				{
					lpBitmapInfoHeader = (LPBITMAPINFOHEADER) lParam1; 
					lpBitmapPixelData = (LPBYTE) lParam2;
					lTimeStamp = (unsigned long) lParam3;
				}
			}
			break;

		default:
			break;
	}	
	wasDisplayed = true;
}

void CTripodDlg::allocateDib(CSize sz)
{
	// Purpose: allocate information for a device independent bitmap (DIB)
	// Called from OnInitVideo

	if(m_destinationBitmapInfoHeader) {
		free(m_destinationBitmapInfoHeader);
		m_destinationBitmapInfoHeader = NULL;
	}

	if(sz.cx | sz.cy) {
		m_destinationBitmapInfoHeader = (LPBITMAPINFOHEADER)malloc(DibSize(sz.cx, sz.cy, 24));
		ASSERT(m_destinationBitmapInfoHeader);
		m_destinationBitmapInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		m_destinationBitmapInfoHeader->biWidth = sz.cx;
		m_destinationBitmapInfoHeader->biHeight = sz.cy;
		m_destinationBitmapInfoHeader->biPlanes = 1;
		m_destinationBitmapInfoHeader->biBitCount = 24;
		m_destinationBitmapInfoHeader->biCompression = 0;
		m_destinationBitmapInfoHeader->biSizeImage = DibImageSize(sz.cx, sz.cy, 24);
		m_destinationBitmapInfoHeader->biXPelsPerMeter = 0;
		m_destinationBitmapInfoHeader->biYPelsPerMeter = 0;
		m_destinationBitmapInfoHeader->biClrImportant = 0;
		m_destinationBitmapInfoHeader->biClrUsed = 0;
	}

	if(m_destinationBitmapRightInfoHeader) {
		free(m_destinationBitmapRightInfoHeader);
		m_destinationBitmapRightInfoHeader = NULL;
	}

	if(sz.cx | sz.cy) {
		m_destinationBitmapRightInfoHeader = (LPBITMAPINFOHEADER)malloc(DibSize(sz.cx, sz.cy, 24));
		ASSERT(m_destinationBitmapRightInfoHeader);
		m_destinationBitmapRightInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
		m_destinationBitmapRightInfoHeader->biWidth = sz.cx;
		m_destinationBitmapRightInfoHeader->biHeight = sz.cy;
		m_destinationBitmapRightInfoHeader->biPlanes = 1;
		m_destinationBitmapRightInfoHeader->biBitCount = 24;
		m_destinationBitmapRightInfoHeader->biCompression = 0;
		m_destinationBitmapRightInfoHeader->biSizeImage = DibImageSize(sz.cx, sz.cy, 24);
		m_destinationBitmapRightInfoHeader->biXPelsPerMeter = 0;
		m_destinationBitmapRightInfoHeader->biYPelsPerMeter = 0;
		m_destinationBitmapRightInfoHeader->biClrImportant = 0;
		m_destinationBitmapRightInfoHeader->biClrUsed = 0;
	}
}

void CTripodDlg::displayMyResults(LPBITMAPINFOHEADER lpThisBitmapInfoHeader)
{
	// displayMyResults: Displays results of doMyImageProcessing() in the videoport
	// Notes: StretchDIBits stretches a device-independent bitmap to the appropriate size

	CDC				*pDC;	// Device context to display bitmap data
	
	pDC = GetDC();	
	int nOldMode = SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);

	StretchDIBits( 
		pDC->GetSafeHdc(),
		m_rectForProcessedView.left,				// videoportal left-most coordinate
		m_rectForProcessedView.top,					// videoportal top-most coordinate
		m_rectForProcessedView.Width(),				// videoportal width
		m_rectForProcessedView.Height(),			// videoportal height
		0,											// Row position to display bitmap in videoportal
		0,											// Col position to display bitmap in videoportal
		lpThisBitmapInfoHeader->biWidth,			// m_destinationBmp's number of columns
		lpThisBitmapInfoHeader->biHeight,			// m_destinationBmp's number of rows
		m_destinationBmp,							// The bitmap to display; use the one resulting from doMyImageProcessing
		(BITMAPINFO*)m_destinationBitmapInfoHeader, // The bitmap's header info e.g. width, height, number of bits etc
		DIB_RGB_COLORS,								// Use default 24-bit color table
		SRCCOPY										// Just display
	);
 
	SetStretchBltMode(pDC->GetSafeHdc(),nOldMode);

	ReleaseDC(pDC);


	// Note: 04/24/02 - Added the following:
	// Christopher Wagner cwagner@fas.harvard.edu noticed that memory wasn't being freed

	// Recall OnPortalNotificationProcessedview, which gets called everytime
	// a frame of data arrives, performs 3 steps:
	// (1) grayScaleTheFrameData - which mallocs m_destinationBmp
	// (2) doMyImageProcesing
	// (3) displayMyResults - which we're in now
	// Since we're finished with the memory we malloc'ed for m_destinationBmp
	// we should free it: 
	
	free(m_destinationBmp);

	// End of adds


}


void CTripodDlg::displayMyResultsRight(LPBITMAPINFOHEADER lpThisBitmapInfoHeader)
{
	// displayMyResults: Displays results of doMyImageProcessing() in the videoport
	// Notes: StretchDIBits stretches a device-independent bitmap to the appropriate size

	CDC				*pDC;	// Device context to display bitmap data
	
	pDC = GetDC();	
	int nOldMode = SetStretchBltMode(pDC->GetSafeHdc(),COLORONCOLOR);

	StretchDIBits( 
		pDC->GetSafeHdc(),
		m_rectForProcessedViewRight.left,				// videoportal left-most coordinate
		m_rectForProcessedViewRight.top,					// videoportal top-most coordinate
		m_rectForProcessedViewRight.Width(),				// videoportal width
		m_rectForProcessedViewRight.Height(),			// videoportal height
		0,											// Row position to display bitmap in videoportal
		0,											// Col position to display bitmap in videoportal
		lpThisBitmapInfoHeader->biWidth,			// m_destinationBmp's number of columns
		lpThisBitmapInfoHeader->biHeight,			// m_destinationBmp's number of rows
		m_destinationBmpRight,						// The bitmap to display; use the one resulting from doMyImageProcessing
		(BITMAPINFO*)m_destinationBitmapRightInfoHeader, // The bitmap's header info e.g. width, height, number of bits etc
		DIB_RGB_COLORS,								// Use default 24-bit color table
		SRCCOPY										// Just display
	);
 
	SetStretchBltMode(pDC->GetSafeHdc(),nOldMode);

	ReleaseDC(pDC);


	// Note: 04/24/02 - Added the following:
	// Christopher Wagner cwagner@fas.harvard.edu noticed that memory wasn't being freed

	// Recall OnPortalNotificationProcessedview, which gets called everytime
	// a frame of data arrives, performs 3 steps:
	// (1) grayScaleTheFrameData - which mallocs m_destinationBmp
	// (2) doMyImageProcesing
	// (3) displayMyResults - which we're in now
	// Since we're finished with the memory we malloc'ed for m_destinationBmp
	// we should free it: 
	
	free(m_destinationBmpRight);

	// End of adds


}


void CTripodDlg::grayScaleTheFrameData(LPBITMAPINFOHEADER lpThisBitmapInfoHeader, LPBYTE lpThisBitmapPixelData)
{
	
	// grayScaleTheFrameData: Called by CTripodDlg::OnPortalNotificationBinaryview
	// Task: Read current frame pixel data and computes a grayscale version

	int	W, H;			  // Width and Height of current frame [pixels]
	BYTE            *sourceBmp;		  // Pointer to current frame of data
	int    row, col;
	unsigned long   i;
	BYTE			grayValue;

	BYTE			redValue;
	BYTE			greenValue;
	BYTE			blueValue;

    W = lpThisBitmapInfoHeader->biWidth;  // biWidth: number of columns
    H = lpThisBitmapInfoHeader->biHeight; // biHeight: number of rows

	// Store pixel data in row-column vector format
	// Recall that each pixel requires 3 bytes (red, blue and green bytes)
	// m_destinationBmp is a protected member and declared in binarizeDlg.h

	m_destinationBmp = (BYTE*)malloc(H*3*W*sizeof(BYTE));

	// Point to the current frame's pixel data
	sourceBmp = lpThisBitmapPixelData;

	for (row = 0; row < H; row++) {
		for (col = 0; col < W; col++) {

			// Recall each pixel is composed of 3 bytes
			i = (unsigned long)(row*3*W + 3*col);
        
			// The source pixel has a blue, green andred value:
			blueValue  = *(sourceBmp + i);
			greenValue = *(sourceBmp + i + 1);
			redValue   = *(sourceBmp + i + 2);

			// A standard equation for computing a grayscale value based on RGB values
			grayValue = (BYTE)(0.299*redValue + 0.587*greenValue + 0.114*blueValue);

			// The destination BMP will be a grayscale version of the source BMP
			*(m_destinationBmp + i)     = grayValue;
			*(m_destinationBmp + i + 1) = grayValue;
			*(m_destinationBmp + i + 2) = grayValue;
			
		}
	}
}


void CTripodDlg::grayScaleTheFrameDataRight(LPBITMAPINFOHEADER lpThisBitmapInfoHeader, LPBYTE lpThisBitmapPixelData)
{
	
	// grayScaleTheFrameData: Called by CTripodDlg::OnPortalNotificationBinaryview
	// Task: Read current frame pixel data and computes a grayscale version

	int	W, H;			  // Width and Height of current frame [pixels]
	BYTE            *sourceBmp;		  // Pointer to current frame of data
	int    row, col;
	unsigned long   i;
	BYTE			grayValue;

	BYTE			redValue;
	BYTE			greenValue;
	BYTE			blueValue;

    W = lpThisBitmapInfoHeader->biWidth;  // biWidth: number of columns
    H = lpThisBitmapInfoHeader->biHeight; // biHeight: number of rows

	// Store pixel data in row-column vector format
	// Recall that each pixel requires 3 bytes (red, blue and green bytes)
	// m_destinationBmp is a protected member and declared in binarizeDlg.h

	m_destinationBmpRight = (BYTE*)malloc(H*3*W*sizeof(BYTE));

	// Point to the current frame's pixel data
	sourceBmp = lpThisBitmapPixelData;

	for (row = 0; row < H; row++) {
		for (col = 0; col < W; col++) {

			// Recall each pixel is composed of 3 bytes
			i = (unsigned long)(row*3*W + 3*col);
        
			// The source pixel has a blue, green andred value:
			blueValue  = *(sourceBmp + i);
			greenValue = *(sourceBmp + i + 1);
			redValue   = *(sourceBmp + i + 2);

			// A standard equation for computing a grayscale value based on RGB values
			grayValue = (BYTE)(0.299*redValue + 0.587*greenValue + 0.114*blueValue);

			// The destination BMP will be a grayscale version of the source BMP
			*(m_destinationBmpRight + i)     = grayValue;
			*(m_destinationBmpRight + i + 1) = grayValue;
			*(m_destinationBmpRight + i + 2) = grayValue;
			
		}
	}
}

void CTripodDlg::doMyImageProcessing(LPBITMAPINFOHEADER lpThisBitmapInfoHeader, int side)
{
	unsigned int	W, H;			// Width and Height of current frame [pixels]
	W = lpThisBitmapInfoHeader->biWidth;  // biWidth: number of columns
	H = lpThisBitmapInfoHeader->biHeight; // biHeight: number of rows
	unsigned int    row, col;		// Pixel's row and col positions
	unsigned long   i;				// Dummy variable for row-column vector
	int rowOffset;					// Row offset from the current pixel
	int colOffset;					// Col offset from the current pixel
	int rowTotal = 0;				// Row position of offset pixel
	int colTotal = 0;				// Col position of offset pixel
	int tempIntensity[20][20];		// Temporary array of pixels to compare one image to the other
	int depthMapReal[240][320];		// The resulting depth map
	int row2;						// Row of other image
	int col2;						// Col of other image
	int tempDepth;					// Temporary storage of depth to convert to color
	int depthRed;					// Red value for a given depth
	int depthGreen;					// Green value for a given depth
	int depthBlue;					// Blue value for a given depth
	long intensityMatch;			// Sum of differences between corresponding pixels in a region
	int increment = 3;				// Number of pixels to shift the current region
	int rowStart;					// Lowest row to compare to
	int bestCol2;					// Column with the closest match
	int bestMatch;					// Intensity match that is the lowest
	int offset = 6;					// Width of the region to compare

	char			str[80];		  // To print message 
	CDC				*pDC;			  // Device context need to print message	
	
	// Clear Depth Map
	for (row = 0; row < H; row++) {
		for (col = 0; col < W; col++) {
			depthMapReal[row][col] = 0;			
		}
	}

	switch (side) {
	case 4:

		// Store current image in an array to compare to the other camera's view
		for (row = 0; row < H; row++) {
			for (col = 0; col < W; col++) {
				i = (unsigned long)(row*3*W + col*3);
				otherImage[row][col] = *(m_destinationBmpRight + i);
			}
		}

		break;


	case 5:
		
		/* Determine depths */	
		for (row = 0; row < H-offset; row+=increment) {
			for (col = 0; col < W-offset; col+=increment) {
				// Set up temporary array of current region's pixels from the other camera
				for (rowOffset=0; rowOffset<offset; rowOffset++) {
					for (colOffset=0; colOffset<offset; colOffset++) {
						i = (unsigned long)((row+rowOffset)*3*W + 3*(col+colOffset));
						tempIntensity[rowOffset][colOffset] = otherImage[row+rowOffset][col+colOffset];
					}
				}
				// Decide where to start
				if(row < H - 5){
					row2 = row + 5;
				}else{
					row2 = H - row;
				}
				rowStart = row2;
				col2 = 0;
				bestCol2 = 0;
				bestMatch = 9999999;
				// Look for a match
				while (row2 > rowStart - 5) {
					while (col2 < W-offset) {
						intensityMatch = 0;
						for (rowOffset=0; rowOffset<offset; rowOffset++) {
							for (colOffset=0; colOffset<offset; colOffset++) {
								i = (unsigned long)((row2+rowOffset)*3*W + 3*(col2+colOffset));
								intensityMatch += abs(tempIntensity[rowOffset][colOffset] - *(m_destinationBmp + i));
							}
						}
						// If the current match is better than the other matches, save it
						if(intensityMatch < bestMatch){
							bestMatch = intensityMatch;	
							bestCol2 = col2;		
						}						
						col2++;
					}
					row2--;
				}

				// Report the closest match per region
				pDC = GetDC();
				sprintf(str, "BestMatch: %d", bestMatch);
				pDC->TextOut(360, 600, str);
				ReleaseDC(pDC);
				
				// Set depth map value if it is a possible real depth
				if(col - bestCol2 > 0 && col - bestCol2 < 127 && bestMatch < 5000){
					depthMapReal[row / increment][col / increment] = 2 * abs(col - bestCol2);
				}
			}
		}

		// Convert the depth map to color and draw it on the left viewport
		for (row = 0; row < H; row++) {
			for (col = 0; col < W; col++) {
				i = (unsigned long)(row*3*W + col*3);
				tempDepth = depthMapReal[row / increment][col / increment];
				if(tempDepth < 43){
					depthRed = tempDepth * 6;
					depthGreen = 0;
					depthBlue = tempDepth * 6;
				}
				if(tempDepth > 42 && tempDepth < 85){
					depthRed = 255 - (tempDepth - 43) * 6;
					depthGreen = 0;
					depthBlue = 255;
				}
				if(tempDepth > 84 && tempDepth < 128){
					depthRed = 0;
					depthGreen = (tempDepth - 85) * 6;
					depthBlue = 255;
				}
				if(tempDepth > 127 && tempDepth < 169){
					depthRed = 0;
					depthGreen = 255;
					depthBlue = 255 - (tempDepth - 128) * 6;
				}
				if(tempDepth > 168 && tempDepth < 212){
					depthRed = (tempDepth - 169) * 6;
					depthGreen = 255;
					depthBlue = 0;
				}
				if(tempDepth > 211 && tempDepth < 254){
					depthRed = 255;
					depthGreen = 255 - (tempDepth - 212) * 6;
					depthBlue = 0;
				}
				if(tempDepth > 253){
					depthRed = 255;
					depthGreen = 0;
					depthBlue = 0;
				}

				*(m_destinationBmp + i) = depthBlue;
				*(m_destinationBmp + i + 1) = depthGreen;
				*(m_destinationBmp + i + 2) = depthRed;
			}
		}

		break;
	}


}


void CTripodDlg::OnTimer(UINT nIDEvent) 
{
	if(countStart < 4)
		countStart++;
	
	if ((whichSide) && (wasDisplayed)) {

		// Change Camera
		m_cVideoUnprocessedView.ConnectCamera(1);

		whichSide = false;
		wasDisplayed = false;

		grayScaleTheFrameData(lpBitmapInfoHeader, lpBitmapPixelData);
		doMyImageProcessing(lpBitmapInfoHeader, countStart + 1); // Place where you'd add your image processing code
		displayMyResults(lpBitmapInfoHeader);
	}
	else if(wasDisplayed) {

		// Change Camera
		m_cVideoUnprocessedView.ConnectCamera(0);

		whichSide = true;
		wasDisplayed = false;

		grayScaleTheFrameDataRight(lpBitmapInfoHeader, lpBitmapPixelData);
		doMyImageProcessing(lpBitmapInfoHeader, countStart); // Place where you'd add your image processing code
		displayMyResultsRight(lpBitmapInfoHeader);
	}


	CDialog::OnTimer(nIDEvent);
}

