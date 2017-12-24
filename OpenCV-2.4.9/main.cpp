#include <opencv.hpp>


int main( int argc, char** argv ) {
    const char *window;                                                   // name of window
    CvCapture* capture;                                                   // video
    IplImage* frame;                                                        // frame


    window = "Camera Input";                                          // reference of camera window
    capture = cvCaptureFromCAM(0);
    cvNamedWindow(window, CV_WINDOW_AUTOSIZE);  // construct a window
    for(;;) {                                                                       // repeat forever
        frame = cvQueryFrame(capture);                              // get the first frame
        if(!frame || cvWaitKey(10) >= 0) {                         // if no frame or a keypress event
            break; // break event
        }
        cvShowImage(window, frame );                               // show the image in window
    }
    cvReleaseCapture(&capture);                                      // destroy captured data
    cvDestroyWindow(window); // destroy referenced window
    return 0;
}
