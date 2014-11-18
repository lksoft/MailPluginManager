//
//  FsprgEmbeddedStoreDelegate.h
//  FsprgEmbeddedStore
//
//  Created by Lars Steiger on 2/22/10.
//  Copyright 2010 FastSpring. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "FsprgStoreParameters.h"
#import "FsprgOrder.h"

/*! Type for didLoadPage:ofType: */
typedef enum {
	FsprgPageFS,
	FsprgPagePayPal,
	FsprgPageUnknown
} FsprgPageType;


/*!
 * Delegate protocol for FsprgEmbeddedStoreController.
 */
@protocol FsprgEmbeddedStoreDelegate <NSObject>

/*!
 * Gets called on initial load of the store.
 * @param url URL of the loaded store
 */
- (void)didLoadStore:(NSURL *)url;

/*!
 * Gets called on subsequent page loads.
 * @param url URL of the loaded page
 * @param pageType Type of page this url is pointing to
 */
- (void)didLoadPage:(NSURL *)url ofType:(FsprgPageType)pageType;

/*!
 * Gets called after finishing the order process.
 * @param order Order information
 */
- (void)didReceiveOrder:(FsprgOrder *)order;

/*!
 * Gets called to present the order confirmation.
 * @param frame The frame rectangle for the view object
 * @param order Order information
 * @result The view presenting the order confirmation
 */
- (NSView *)viewWithFrame:(NSRect)frame forOrder:(FsprgOrder *)order;

/*!
 * Invoked if an error occurs when starting to load data for a page.
 * @param sender The web view containing the frame.
 * @param error Specifies the type of error that occurred during the load.
 * @param frame The frame being loaded.
 */
- (void)webView:(WebView *)sender didFailProvisionalLoadWithError:(NSError *)error forFrame:(WebFrame *)frame;

/*!
 * Invoked when an error occurs loading a committed data source.
 * @param sender The web view containing the frame.
 * @param error The type of error that occurred during the load.
 * @param frame The frame being loaded. 
 */
- (void)webView:(WebView *)sender didFailLoadWithError:(NSError *)error forFrame:(WebFrame *)frame;

@end
