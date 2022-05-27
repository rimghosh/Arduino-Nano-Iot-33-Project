import boto3


#
#   expects event parameter to contain:
#{
#  "controller": "Nano 33 IOT",
#  "time": 1652842210,
#  "UpTime": 1954372,
#  "weight": 0.000303111,
#  "Temperature": 72.86000061,
#  "Humidity": 52.40000153
#}
#   sends a plain text string to be used in a text message



def lambda_handler(event, context):

    # Create an SNS client to send notification
    sns = boto3.client('sns')

    # Format text message from data
    message_text = "Device {} reports a weight of {}, which is below the limit. To reorder please use the following link https://www.amazon.com/rice/s?k=rice".format(
        str(event['controller']),
        float(event['weight'])
        )


    # Publish the formatted message
    response = sns.publish(
            TopicArn = "arn:aws:sns:us-east-1:xxxxxxxxxxxx:new-topic",
            Message = message_text
        )

    return response
