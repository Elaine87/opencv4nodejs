#include "macros.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "Mat.h"

class Io {
public:
  static NAN_MODULE_INIT(Init);
  static NAN_METHOD(Imread);
  static NAN_METHOD(Imwrite);
  static NAN_METHOD(Imshow);
	static NAN_METHOD(ImshowWait);
	static NAN_METHOD(WaitKey);
	static NAN_METHOD(Imencode);
	static NAN_METHOD(Imdecode);
	static NAN_METHOD(MoveWindow);
	static NAN_METHOD(DestroyWindow);
	static NAN_METHOD(DestroyAllWindows);

	static NAN_METHOD(ImreadAsync);
	static NAN_METHOD(ImwriteAsync);
	static NAN_METHOD(ImencodeAsync);
	static NAN_METHOD(ImdecodeAsync);

	struct ImreadContext {
	public:
		std::string path;
		int flags = cv::IMREAD_COLOR;
		cv::Mat img;

		const char* execute() {
			img = cv::imread(path, flags);
			if (img.rows == 0 && img.cols == 0) {
				return("empty Mat");
			}
			return "";
		}

		FF_VAL getReturnValue() {
			FF_OBJ jsImg = FF_NEW_INSTANCE(Mat::constructor);
			FF_UNWRAP_MAT_AND_GET(jsImg) = img;
			return jsImg;
		}
	};

	struct ImwriteContext {
	public:
		std::string path;
		std::vector<int> flags = std::vector<int>();
		cv::Mat img;

		const char* execute() {
			cv::imwrite(path, img);
			return "";
		}

		FF_VAL getReturnValue() {
			return Nan::Undefined();
		}
	};

	struct ImencodeContext {
	public:
		std::string ext;
		cv::Mat img;
		std::vector<int> flags;
		char *data;
		size_t dataSize;

		const char* execute() {
			std::vector<uchar> dataVec;
			cv::imencode(ext, img, dataVec, flags);
			dataSize = dataVec.size() * sizeof(char);
			data = static_cast<char *>(malloc(dataSize));
			memcpy(data, reinterpret_cast<char*>(dataVec.data()), dataSize);
			return "";
		}

		FF_VAL getReturnValue() {
			FF_OBJ jsBuf = Nan::NewBuffer(data, dataSize).ToLocalChecked();
			return jsBuf;
		}
	};

	struct ImdecodeContext {
	public:
		int flags;
		cv::Mat img;
		char *data;
		size_t dataSize;


		const char* execute() {
			std::vector<uchar> vec(dataSize);
			memcpy(vec.data(), data, dataSize);
			img = cv::imdecode(vec, flags);
			return "";
		}

		FF_VAL getReturnValue() {
			FF_OBJ jsDecodedMat = FF_NEW_INSTANCE(Mat::constructor);
			FF_UNWRAP_MAT_AND_GET(jsDecodedMat) = img;
			return jsDecodedMat;
		}
	};
};
