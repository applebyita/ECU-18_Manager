#include "QtIosNotifier.h"

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <UserNotifications/UserNotifications.h>

@implementation NotificationDelegate
        
- (void)showNotification:(NSString*)title
                          message:(NSString*)msg;
{
UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];

    if (center)
    {
        //request notification permissions
        UNAuthorizationOptions options = UNAuthorizationOptionAlert + UNAuthorizationOptionSound;
        [center requestAuthorizationWithOptions:options
         completionHandler:^(BOOL granted, NSError * _Nullable error) {
          if (!granted) {
            NSLog(@"Oops, no access");
          }
        }];

        //create notification
        UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
        content.title = title;
        content.body = msg;
        content.sound = [UNNotificationSound defaultSound];

        //create trigger
        UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:1 repeats:NO];

        //add notification request
        UNNotificationRequest* notificationRequest = [UNNotificationRequest requestWithIdentifier:@"ECU-18 Manager" content:content trigger:trigger];

        [center addNotificationRequest:notificationRequest withCompletionHandler:^(NSError * _Nullable error) {
            if (error != nil) {
                NSLog(@"Unable to Add Notification Request");
            }
        }];

    }




//UILocalNotification *notification = [[UILocalNotification alloc] init];
//  if (notification)
//  {
//          notification.fireDate = [NSDate dateWithTimeIntervalSinceNow:10]; // adds 10 secs
//          notification.alertTitle = title;
//          notification.alertBody = msg;

//          notification.timeZone = [NSTimeZone defaultTimeZone];
//          notification.applicationIconBadgeNumber = 1;
//          notification.soundName = UILocalNotificationDefaultSoundName;
//          notification.repeatInterval = 0;

//          // this will schedule the notification to fire at the fire date
//          [[UIApplication sharedApplication] scheduleLocalNotification:notification];

////          // this will fire the notification right away, it will still also fire at the date we set
////          [[UIApplication sharedApplication] presentLocalNotificationNow:notification];
//    }
////    UILocalNotification* localNotification = [[UILocalNotification alloc] init];
////    localNotification.fireDate = [NSDate dateWithTimeIntervalSinceNow:10];
////    localNotification.alertBody = msg;
////    localNotification.timeZone = [NSTimeZone defaultTimeZone];
////    [[UIApplication sharedApplication] scheduleLocalNotification:localNotification];
}

@end
    
//------------------------------------------------------------------------
QtIosNotifier::QtIosNotifier()
{
//    // New for iOS 8 - Register the notifications
//            UIUserNotificationType types = UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert;
//            UIUserNotificationSettings *mySettings = [UIUserNotificationSettings settingsForTypes:types categories:nil];
//            [[UIApplication sharedApplication] registerUserNotificationSettings:mySettings];
}

//------------------------------------------------------------------------
bool QtIosNotifier::show(QString title, QString caption, int Badge)
{
    //    NSLog(@"Notification in 10 sec");

        {
            UNUserNotificationCenter *center = [UNUserNotificationCenter currentNotificationCenter];

                if (center)
                {
                    //NSString* title=@"Ciao";
                    //NSString* body=@"Dai!!!!";
                    //request notification permissions
                    UNAuthorizationOptions options = UNAuthorizationOptionAlert + UNAuthorizationOptionSound;
                    [center requestAuthorizationWithOptions:options
                     completionHandler:^(BOOL granted, NSError * _Nullable error) {
                      if (!granted) {
                        NSLog(@"Oops, no access");
                      }
                    }];

                    //create notification
                    UNMutableNotificationContent *content = [[UNMutableNotificationContent alloc] init];
                    content.title = title.toNSString();
                    content.body = caption.toNSString();
                    content.sound = [UNNotificationSound defaultSound];

                    //create trigger
                    UNTimeIntervalNotificationTrigger *trigger = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:5 repeats:NO];

                    //add notification request
                    UNNotificationRequest* notificationRequest = [UNNotificationRequest requestWithIdentifier:@"ECU-18 Manager" content:content trigger:trigger];

                    [center addNotificationRequest:notificationRequest withCompletionHandler:^(NSError * _Nullable error) {
                        if (error != nil) {
                            NSLog(@"Unable to Add Notification Request");
                        }
                    }];
                }
        }
 //   NotificationDelegate *mn = [[[NotificationDelegate alloc] init] autorelease];
 //   [mn showNotification:title.toNSString() message:caption.toNSString()];
    return true;
}
//------------------------------------------------------------------------


