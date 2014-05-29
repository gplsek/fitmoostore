<?php

/**
 * This file is part of the Geocoder package.
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 *
 * @license    MIT License
 */

namespace Geocoder\HttpAdapter;

use Geocoder\Exception\ExtensionNotLoadedException;

/**
 * @author William Durand <william.durand1@gmail.com>
 */
class CurlHttpAdapter implements HttpAdapterInterface
{
    private $timeout;

    private $connectTimeout;

    public function __construct($timeout = null, $connectTimeout = null)
    {
        $this->timeout = $timeout;
        $this->connectTimeout = $connectTimeout;
    }

    /**
     * {@inheritDoc}
     */
    public function getContent($url)
    {
        if (!function_exists('curl_init')) {
            throw new ExtensionNotLoadedException('cURL has to be enabled.');
        }
        $agent = 'Mozilla/5.0 (Windows NT 6.2; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/35.0.1916.114 Safari/537.36';
        $c = curl_init();

        curl_setopt($c, CURLOPT_USERAGENT, $agent);
        curl_setopt($c, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($c, CURLOPT_FOLLOWLOCATION, true);
        curl_setopt($c, CURLOPT_VERBOSE, false);
        curl_setopt($c, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($c, CURLOPT_URL, $url);
        curl_setopt($c, CURLOPT_TIMEOUT, 20);
        curl_setopt($c, CURLOPT_CONNECTTIMEOUT, 20);

        $content = curl_exec($c);
        curl_close($c);

        if (false === $content) {
            $content = null;
        }

        return $content;
    }

    /**
     * {@inheritDoc}
     */
    public function getName()
    {
        return 'curl';
    }
}
