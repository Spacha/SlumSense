<?php

namespace App\Events;

use Illuminate\Broadcasting\Channel;
use Illuminate\Broadcasting\InteractsWithSockets;
use Illuminate\Broadcasting\PresenceChannel;
use Illuminate\Broadcasting\PrivateChannel;
use Illuminate\Contracts\Broadcasting\ShouldBroadcast;
use Illuminate\Foundation\Events\Dispatchable;
use Illuminate\Queue\SerializesModels;

use App\Models\Measurement;

/**
 * Pushes the latest, just stored measurement to the channel.
 */
class MeasurementStored implements ShouldBroadcast
{
    use Dispatchable, InteractsWithSockets, SerializesModels;

    public $measurement;

    /**
     * Create a new event instance.
     *
     * @return void
     */
    public function __construct(Measurement $measurement)
    {
        // $this->measurements = Measurement::all();
        $this->measurement = $measurement;
    }

    /**
     * Get the channels the event should broadcast on.
     *
     * @return \Illuminate\Broadcasting\Channel|array
     */
    public function broadcastOn()
    {
        //dd('asasassak');
        // return new PrivateChannel('channel-name');
        return new Channel('measurements');
    }
}
