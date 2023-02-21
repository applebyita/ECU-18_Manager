#import "AppDelegate.h"
#include "iosappstate.h"

#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>

#import <Foundation/Foundation.h>
#import <UserNotifications/UserNotifications.h>

#import <QtAppDelegate-C-Interface.h>

@interface QIOSApplicationDelegate

@end

@interface QIOSApplicationDelegate(MyApp)

@end

static QApplication* pQApp;
static MainWindow*   pMainWindow;

@implementation QIOSApplicationDelegate(MyApp)

- (void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
//qDebug() << "Lets release some memory before we get killed";
}

- (void)didReceiveLocalNotification:(UILocalNotification *)notification
{
UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:@"Notification Received" message:notification.alertBody delegate:nil     cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
[alertView show];
}

//@interface AppDelegate : UIResponder <UIApplicationDelegate>

//@property (strong, nonatomic) UIWindow *window;

//@end

//@implementation AppDelegate

//- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
//{
//    // Override point for customization after application launch.
//    return YES;
//}

//- (void)applicationWillResignActive:(UIApplication *)application
//{
//    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
//    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
//}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.

    IOSAppState::getInstance()->applicationDidEnterBackGround();
}

//- (void)applicationWillEnterForeground:(UIApplication *)application
//{
//    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
//}

//- (void)applicationDidBecomeActive:(UIApplication *)application
//{
//    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
//}

//- (void)applicationWillTerminate:(UIApplication *)application
//{
//    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.

//}

//-(BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
//    [[UIApplication sharedApplication] setMinimumBackgroundFetchInterval:UIApplicationBackgroundFetchIntervalMinimum];
//    return true;
//}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  //  [[UIApplication sharedApplication] setMinimumBackgroundFetchInterval:UIApplicationBackgroundFetchIntervalMinimum];
    [[UIApplication sharedApplication] setMinimumBackgroundFetchInterval:30];

    UIBackgroundRefreshStatus status = [[UIApplication sharedApplication] backgroundRefreshStatus];
    switch (status) {
        case UIBackgroundRefreshStatusAvailable:
        // We can do background fetch! Let's do this!
        break;
        case UIBackgroundRefreshStatusDenied:
        // The user has background fetch turned off. Too bad.
        break;
        case UIBackgroundRefreshStatusRestricted:
        // Parental Controls, Enterprise Restrictions, Old Phones, Oh my!
        break;
    }

    return YES;
}

//- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
//{
//UIAlertView *alertView = [[UIAlertView alloc]initWithTitle:@"Notification Received" message:notification.alertBody delegate:nil     cancelButtonTitle:@"OK" otherButtonTitles:nil, nil];
//[alertView show];
//}

-(void)application:(UIApplication *)application performFetchWithCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    NSLog(@"Background fetch started...");

    //---do background fetch here---


//    NSLog(@"Notification in 10 sec");

//    {
//        UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];

//            if (center)
//            {
//                NSString* title=@"Ciao";
//                NSString* body=@"Dai!!!!";
//                //request notification permissions
//                UNAuthorizationOptions options = UNAuthorizationOptionAlert + UNAuthorizationOptionSound;
//                [center requestAuthorizationWithOptions:options
//                 completionHandler:^(BOOL granted, NSError * _Nullable error) {
//                  if (!granted) {
//                    NSLog(@"Oops, no access");
//                  }
//                }];

//                //create notification
//                UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
//                content.title = title;
//                content.body = body;
//                content.sound = [UNNotificationSound defaultSound];

//                //create trigger
//                UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:10 repeats:NO];

//                //add notification request
//                UNNotificationRequest* notificationRequest = [UNNotificationRequest requestWithIdentifier:@"ECU-18 Manager" content:content trigger:trigger];

//                [center addNotificationRequest:notificationRequest withCompletionHandler:^(NSError * _Nullable error) {
//                    if (error != nil) {
//                        NSLog(@"Unable to Add Notification Request");
//                    }
//                }];

//            }
//    }

    // You have up to 30 seconds to perform the fetch

    pMainWindow->RichiediElencoMacchine();

    while (pMainWindow->hHTTP.ComandoAttuale!=NESSUN_COMANDO){
            QApplication::processEvents();
    }

    BOOL downloadSuccessful = YES;

    if (downloadSuccessful) {
        //---set the flag that data is successfully downloaded---
        completionHandler(UIBackgroundFetchResultNewData);
    } else {
        //---set the flag that download is not successful---
        completionHandler(UIBackgroundFetchResultFailed);
    }

    NSLog(@"Background fetch completed...");

////    NSUserDefaults *standardDefaults = [NSUserDefaults standardUserDefaults];

////    // Get Current Location from NSUserDefaults
////    CLLocationCoordinate2D currentLocation;
////    currentLocation.latitude = [standardDefaults floatForKey:@"locationLatitude"];
////    currentLocation.longitude = [standardDefaults floatForKey:@"locationLongitude"];

////    // GetWeather for current location
////    GetWeather *getWeather = [[GetWeather alloc] init];
////    [getWeather getWeatherAtCurrentLocation:currentLocation];

////    // Set up Local Notifications
////    [[UIApplication sharedApplication] cancelAllLocalNotifications];
////    UILocalNotification *localNotification = [[UILocalNotification alloc] init];
////    NSDate *now = [NSDate date];
////    localNotification.fireDate = now;
////    localNotification.alertBody = [NSString stringWithFormat:@"Temperature in %@ is %@.", getWeather.currentLocation, getWeather.currentTemperature];
////    localNotification.soundName = UILocalNotificationDefaultSoundName;
////    localNotification.applicationIconBadgeNumber = [getWeather.currentTemperature intValue];
////    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];

////    completionHandler(UIBackgroundFetchResultNewData);
    NSLog(@"Fetch completed");
}

//void (^fetchCompletionHandler)(UIBackgroundFetchResult);
//NSDate *fetchStart;
//UIBackgroundTaskIdentifier backgroundTask;

//-(void)application:(UIApplication *)application performFetchWithCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler
//{
//    NSLog(@"Background fetch started...");
//    fetchCompletionHandler = completionHandler;

//    fetchStart = [NSDate date];

//    self.backgroundTask = [application beginBackgroundTaskWithExpirationHandler:^{
//        [application endBackgroundTask:self.backgroundUpdateTask];
//        self.backgroundTask = UIBackgroundTaskInvalid;
//    }];

//    [[NSUserDefaults standardUserDefaults] setObject:fetchStart forKey:kLastCheckedContentDate];

//    [FeedParser parseFeedAtUrl:url withDelegate:self];
////    NSLog(@"Fetch completed");
//}

//-(void)onParserFinished
//{
//    DDLogVerbose(@"AppDelegate/onParserFinished");

//    UIBackgroundFetchResult result = UIBackgroundFetchResultNoData;

//    NSDate *fetchEnd = [NSDate date];
//    NSTimeInterval timeElapsed = [fetchEnd timeIntervalSinceDate:fetchStart];
//    DDLogVerbose(@"Background Fetch Duration: %f seconds", timeElapsed);
//    if ([self.mostRecentContentDate compare:item.date] < 0) {
//        DDLogVerbose(@"got new content: %@", item.date);
//        self.mostRecentContentDate = item.date;
//        [self scheduleNotificationWithItem:item];
//        result = UIBackgroundFetchResultNewData;
//    }
//    else {
//        DDLogVerbose(@"no new content.");
//        UILocalNotification* localNotification = [[UILocalNotification alloc] init];
//        localNotification.alertBody = [NSString stringWithFormat:@"Checked for new posts in %f seconds", timeElapsed];
//        [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
//    }
//    fetchCompletionHandler(result);
//    [[UIApplication sharedApplication] application endBackgroundTask:self.backgroundUpdateTask];
//    self.backgroundTask = UIBackgroundTaskInvalid;
//}

void QtAppDelegateInitialize(QApplication* QTapp, MainWindow* pmainwindow){
    pQApp=QTapp;
    pMainWindow=pmainwindow;
    NSLog(@"Indirizzo passato!");
}
@end
