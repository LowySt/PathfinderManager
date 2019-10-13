//
//  DrawView.h
//  TheTruePCMan
//
//  Created by Lowy on 13/10/2019.
//  Copyright © 2019 Lowy. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface DrawView : NSView {

    NSBezierPath *path;
    
}

- (void)setPath;

@end

NS_ASSUME_NONNULL_END
