//
//  XYFunction.h
//  TWP_SkyBookShelf
//
//  Created by Heaven on 13-7-10.
//
//

// 一大堆零散的方法
#import "XYPrecompile.h"

@class MBProgressHUD;
@class ASIHTTPRequest;


/** 
 * 移魂大法
 * api parameters 说明
 * c 类
 * original 原方法
 * replacement 劫持后的方法
 */
static Method XY_swizzleInstanceMethod(Class c, SEL original, SEL replacement) {
    Method a = class_getInstanceMethod(c, original);
    Method b = class_getInstanceMethod(c, replacement);
    // class_addMethod 为该类增加一个新方法
    if (class_addMethod(c, original, method_getImplementation(b), method_getTypeEncoding(b)))
    {
        // 替换类方法的定义
        class_replaceMethod(c, replacement, method_getImplementation(a), method_getTypeEncoding(a));
        return b;   // 返回劫持前的方法
    }
    else
    {
        // 交换2个方法的实现
        method_exchangeImplementations(a, b);
        return b;   // 返回劫持前的方法
    }
    return nil;   //
}

/********************************           Common          *****************/
@interface XYCommon : NSObject{
 
}


typedef enum {
    filePathOption_documents = 1,
    filePathOption_tmp,
    filePathOption_app,
    filePathOption_resource,
} FilePathOption;
/** 
 * 返回文件路径的方法
 * api parameters 说明
 *
 * file 文件名
 * kType 文件所在目录类型. documents:documents文件夹, tmp:Tmp文件夹, app,resource:app文件夹
 */
+(NSString *) dataFilePath:(NSString *)file ofType:(FilePathOption)kType;

/** 
 * 创建目录
 * XYsandbox
 * api parameters 说明
 * aPath 目录路径
 */
+(void) createDirectoryAtPath:(NSString *)aPath;


/**
 * Unicode格式的字符串编码转成中文的方法(如\u7E8C)转换成中文,unicode编码以\u开头
 * NSString(XY)
 * api parameters 说明
 *
 * unicodeStr 需要被转的字符串
 */
+(NSString *) replaceUnicode:(NSString *)unicodeStr;


typedef enum {
    MarkOption_middle = 1,
    MarkOption_front,
    MarkOption_back,
} MarkOption;
/**
 * 返回字符串的位置的方法
 * rangeOfString:返回range. rangeArrayOfString:返回range数组
 * api parameters 说明
 *
 * str 在str中查找
 * iStart 查找起始位置
 * strMark 需要查找的字符串的标记
 * strStart 起始标记
 * strEnd 结束标记
 * operation 模式. MarkOption_middle mark在Start和end中间,当Start=mark时,返回 Start和end中间的Range
 *                MarkOption_front: mark在Start和end前面   MarkOption_back: mark在Start和end后面
 * block 每一个字符串都执行该block
 */
+(NSRange) rangeOfString:(NSString *)str pointStart:(int)iStart start:(NSString *)strStart end:(NSString *)strEnd mark:(NSString *)strMark operation:(MarkOption)operation;
+(NSMutableArray *) rangeArrayOfString:(NSString *)str pointStart:(int)iStart start:(NSString *)strStart end:(NSString *)strEnd mark:(NSString *)strMark operation:(MarkOption)operation;
+(NSMutableArray *) rangeArrayOfString:(NSString *)str pointStart:(int)iStart start:(NSString *)strStart end:(NSString *)strEnd mark:(NSString *)strMark operation:(MarkOption)operation everyStringExecuteBlock:(void(^)(NSRange rangeEvery))block;



#define XYCommon_lastLocation -1
/**
 * 返回没有属性的xml中指定节点的值的方法
 * api parameters 说明
 *
 * str xml字符串
 * akey 节点名
 * location 起始位置
 * operation 模式. XYCommon_lastLocation 从上次结束的位置开始查找,提高效率
 */
+(NSString *) getValueInANonAttributeXMLNode:(NSString *)str key:(NSString *)akey location:(int)location;


/**
 * 提取视图层次结构的方法
 * api parameters 说明
 *
 * aView 要提取的视图
 * indent 层次 请给0值
 * outstring 保存层次的字符串
 */
+(void) dumpView: (UIView *) aView atIndent: (int) indent into:(NSMutableString *)outstring;
// 打印视图层次结构
+(NSString *) displayViews: (UIView *) aView;


/**
 * 正则表达式分析字符串的方法
 * analyseString:返回NSString数组. analyseStringToRange:返回range数组
 * api parameters 说明
 *
 * str 被分析的字符串
 * regexStr 用于分析str的正则表达式 (.|[\r\n])*? 表示任何多个字符，包括换行符，懒惰扫描
 * (已取消)options 匹配选项使用
 */
+(NSMutableArray *) analyseString:(NSString *)str regularExpression:(NSString *)regexStr;
+(NSMutableArray *) analyseStringToRange:(NSString *)str regularExpression:(NSString *)regexStr;

#pragma mark todo, 分享facebook,发EMail
/**
 * 分享至Twitter的方法
 * api parameters 说明
 *
 * strText 需要分享的文字
 * picPath 图片路径
 * strURL URL地址
 * vc Twitter的父视图控制器,目前版本请用nil,默认为[UIApplication sharedApplication].delegate.window.rootViewControlle
 */
+(void) shareToTwitterWithStr:(NSString *)strText withPicPath:(NSString *)picPath withURL:(NSString*)strURL inController:(id)vc;


/**
 * 返回UUID
 * api parameters 说明
 */
+(NSString *) UUID;
// 没有"-"
+(NSString *) UUIDWithoutMinus;

/**
 * 用打开一个URL
 * api parameters 说明
 *
 * url http:// 浏览器, mailto:// 邮件, tel:// 拨号, sms: 短信
 */
+(void) openURL:(NSURL *)url;

/**
 * 得到当前 UIViewController
 * api parameters 说明
 *
 */
+ (UIViewController *) topMostController;


#define SHOWMSG(title, msg, cancel) [XYCommon showAlertViewTitle:title message:msg cancelButtonTitle:cancel];
/**
 * 显示UIAlertView
 * api parameters 说明
 *
 * aTitle msg标题
 * msg 信息
 * strCancel 取消按钮标题
 */
+(void) showAlertViewTitle:(NSString *)aTitle message:(NSString *)msg cancelButtonTitle:(NSString *)strCancel;

/**
 * 替换string里面的单引号'为2个单引号'',用于处理SQL问题
 */
+(NSString *) StringForSQL:(NSString *)str;

/**
 * 返回日期格式器
 * api parameters 说明
 * dateFormatter yyyy-MM-dd HH:mm:ss
 * dateFormatterTemp 很多地方共用的
 * dateFormatterByUTC 返回UTC格式的
 */
+(NSDateFormatter *) dateFormatter;
+(NSDateFormatter *) dateFormatterTemp;
+(NSDateFormatter *) dateFormatterByUTC;
/**
 * 打印内存情况
 * api parameters 说明
 * mark 标记
 */
+(void) printUsedAndFreeMemoryWithMark:(NSString *)mark;


/** 
 * 版本号比大小
 * api parameters 说明
 * 比较oldVersion和newVersion，如果oldVersion比newVersion旧，则返回YES，否则NO
 * Version format[X.X.X]
 */
+(BOOL) compareVersionFromOldVersion:(NSString *)oldVersion newVersion:(NSString *)newVersion;


#pragma mark - to do
/** 
 * objc to dictionary
 * api parameters 说明
 * anObject 对象
 */
//+(NSMutableDictionary *) dictionaryOfObject:(id)anObject;

@end
