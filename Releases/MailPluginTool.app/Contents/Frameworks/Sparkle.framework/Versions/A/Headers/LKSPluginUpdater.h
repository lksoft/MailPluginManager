//
//  LKSPluginUpdater.h
//  Sparkle
//
//  Created by Scott Little on 3/7/13.
//
//

#import <Sparkle/Sparkle.h>

@interface LKSPluginUpdater : SUUpdater {
	BOOL	_skipPreferenceSaves;
}

@property	(assign)	BOOL	skipPreferenceSaves;

@end
