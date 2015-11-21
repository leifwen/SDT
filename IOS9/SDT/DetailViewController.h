//
//  DetailViewController.h
//  SDT
//
//  Created by leif wen on 11/10/15.
//  Copyright © 2015 leif wen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DetailViewController : UIViewController

@property (strong, nonatomic) id detailItem;
@property (weak, nonatomic) IBOutlet UILabel *detailDescriptionLabel;

@end

